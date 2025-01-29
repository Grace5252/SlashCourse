// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "SlashCourseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
struct FInputActionValue;
class AItem;
class UAnimMontage;

UCLASS()
class SLASHCOURSE_API ASlashCourseCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCourseCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

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
private:
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
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
