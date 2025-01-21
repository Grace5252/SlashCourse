// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Treasure.h"
#include "Characters/SlashCourseCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASlashCourseCharacter* SlashCourseCharacter = Cast<ASlashCourseCharacter>(OtherActor))
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}
}
