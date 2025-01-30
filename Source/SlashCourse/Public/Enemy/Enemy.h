// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;
class AWeapon;
class ASoul;

UCLASS()
class SLASHCOURSE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** <AActor> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** <IHitInterface> */
	
protected:
	virtual void BeginPlay() override;

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	/** <ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
private:
	/**
	* Components and WeaponClass to Attach
	*/
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	
	UPROPERTY()
	AAIController* EnemyController;

	//Major Functions
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	bool InTargetRange(AActor* Target, double Radius);
	void CheckPatrolTarget();
	void CheckCombatTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	//Helper Functions
	void PatrolTimerFinished();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	
	bool IsDead();
	bool IsChasing();
	bool IsEngaged();
	bool IsAttacking();

	void StartChasing();
	void StartPatrolling();
	
	bool IsInsideAttackRadius();
	bool IsOutsideAttackRadius();
	bool IsOutsideCombatRadius();

	void InitializeEnemy();
	void SpawnDefaultWeapon();
	void SpawnSoul();
	void LoseInterest();
	void HideHealthBar();
	void ShowHealthBar();
	
	/**
	* AI Navigation Variables
	*/
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChasingSpeed = 300.f;

	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTimeMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTimeMax = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ASoul> SoulClass;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitTimeMin = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitTimeMax = 10.0f;

	//Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;
	
	//All Patrol Targets
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
};
