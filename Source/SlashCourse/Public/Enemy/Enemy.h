// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UPawnSensingComponent;
class AAIController;

UCLASS()
class SLASHCOURSE_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	void PatrolTimerFinished();
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	bool InTargetRange(AActor* Target, double Radius);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void Die();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	//Play Montage Functions
	void PlayHitReactMontage(const FName& SectionName);
private:
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	UParticleSystem* HitParticles;

	/**
	* Navigation
	*/
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double AttackingRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitTimeMin = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitTimeMax = 10.0f;

	FTimerHandle PatrolTimer;

	UPROPERTY()
	AAIController* EnemyController;

	//Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
};
