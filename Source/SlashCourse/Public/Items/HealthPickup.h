// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPickup.generated.h"


UCLASS()
class SLASHCOURSE_API AHealthPickup : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "HealthPickup Properties")
	float Health;
public:
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float HealthCount) { Health = HealthCount; }
};
