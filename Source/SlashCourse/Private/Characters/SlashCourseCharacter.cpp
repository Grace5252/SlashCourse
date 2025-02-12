// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/SlashCourseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/HealthPickup.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashCourseHUD.h"
#include "HUD/SlashCourseOverlay.h"

ASlashCourseCharacter::ASlashCourseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCourseCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ASlashCourseCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashCourseOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SlashCourseOverlay->SetSoulCount(Attributes->GetSouls());
	}
}

void ASlashCourseCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashCourseOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashCourseOverlay->SetCoinCount(Attributes->GetGold());
	}
}

void ASlashCourseCharacter::AddHealth(AHealthPickup* HealthPickup)
{
	if (Attributes && SlashCourseOverlay)
	{
		Attributes->AddHealth(HealthPickup->GetHealth());
		SlashCourseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ASlashCourseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EngageableTarget"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		InitializeSlashCourseOverlay(PlayerController);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashCharacterMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ASlashCourseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCourseCharacter::Dodge);
	}
}

void ASlashCourseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes && SlashCourseOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashCourseOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

float ASlashCourseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCourseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (Attributes && Attributes->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
	}
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ASlashCourseCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCourseCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCourseCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCourseCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;

	DisableMeshCollision();
}

void ASlashCourseCharacter::PlayEquipMontage(const FName& SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
		}
	}
}

void ASlashCourseCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCourseCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCourseCharacter::FinishEquippingWeapon()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCourseCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCourseCharacter::CanAttachToBack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCourseCharacter::CanAttachToHand()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCourseCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedMelee)
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	else
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCourseCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCourseCharacter::EquipWeapon(AWeapon* OverlappingWeapon)
{
	OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

	if (OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_OneHandedMelee)
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
	else
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
	}

	EquippedWeapon = OverlappingWeapon;
	OverlappingItem = nullptr;
}

void ASlashCourseCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCourseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASlashCourseCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void ASlashCourseCharacter::EKeyPressed()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanAttachToBack())
		{
			Disarm();
		}
		else if (CanAttachToHand())
		{
			Arm();
		}
	}
}

void ASlashCourseCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCourseCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodging;
	if (Attributes && SlashCourseOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashCourseOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}



void ASlashCourseCharacter::InitializeSlashCourseOverlay(APlayerController* PlayerController)
{
	if (ASlashCourseHUD* SlashCourseHUD = Cast<ASlashCourseHUD>(PlayerController->GetHUD()))
	{
		SlashCourseOverlay = SlashCourseHUD->GetSlashCourseOverlay();
		if (SlashCourseOverlay && Attributes)
		{
			SlashCourseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			SlashCourseOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			SlashCourseOverlay->SetCoinCount(0);
			SlashCourseOverlay->SetSoulCount(0);
		}
	}
}

void ASlashCourseCharacter::SetHUDHealth()
{
	if (SlashCourseOverlay && Attributes)
	{
		SlashCourseOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

bool ASlashCourseCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

bool ASlashCourseCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCourseCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetCurrentStamina() > Attributes->GetDodgeCost();
}