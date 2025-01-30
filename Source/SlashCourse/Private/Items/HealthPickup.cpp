// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthPickup.h"
#include "Interfaces/PickupInterface.h"

void AHealthPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInterface->AddHealth(this);

		SpawnPickupSystem();
		SpawnPickupSound();
		Destroy();
	}
}
