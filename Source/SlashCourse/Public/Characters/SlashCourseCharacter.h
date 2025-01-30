// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCourseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
struct FInputActionValue;
class AItem;
class ASoul;
class ATreasure;
class AHealthPickup;
class UAnimMontage;
class USlashCourseOverlay;

UCLASS()
class SLASHCOURSE_API ASlashCourseCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCourseCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** <IHitInterface> */

	/** <IPickupInterface>*/
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealthPickup* Health) override;
	/** <IPickupInterface>*/
protected:
	virtual void BeginPlay() override;
	
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual void Die() override;

	/* Play Montage Functions */
	void PlayEquipMontage(const FName& SectionName);

	/* Combat */
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void FinishEquippingWeapon();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	bool CanAttachToBack();
	bool CanAttachToHand();
	void Arm();
	void Disarm();
	
	//Helper Function for EKeyPressed
	void EquipWeapon(AWeapon* OverlappingWeapon);

	//Callbacks for Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	

	/* Input Mapping and Action Variables*/
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* SlashCharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;
private:
	/* Overlay */
	void InitializeSlashCourseOverlay(APlayerController* PlayerController);
	void SetHUDHealth();

	/* Helper Functions */
	bool IsOccupied();
	bool IsUnoccupied();
	bool HasEnoughStamina();
	
	UPROPERTY()
	USlashCourseOverlay* SlashCourseOverlay;

	/* States and Components */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = "Hair")
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;
public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
