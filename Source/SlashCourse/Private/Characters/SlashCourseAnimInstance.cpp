// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCourseAnimInstance.h"
#include "Characters/SlashCourseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashCourseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	SlashCourseCharacter = Cast<ASlashCourseCharacter>(TryGetPawnOwner());
	if (SlashCourseCharacter)
	{
		SlashCourseCharacterMovement = SlashCourseCharacter->GetCharacterMovement();
	}
}

void USlashCourseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCourseCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCourseCharacterMovement->Velocity);
		IsFalling = SlashCourseCharacterMovement->IsFalling();
		CharacterState = SlashCourseCharacter->GetCharacterState();
	}
}
