// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::RecieveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
}

float UAttributeComponent::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return CurrentStamina / MaxStamina;
}

bool UAttributeComponent::IsAlive()
{
	return CurrentHealth > 0.f;
}

void UAttributeComponent::AddGold(int32 GoldCount)
{
	Gold += GoldCount;
}

void UAttributeComponent::AddSouls(int32 SoulsCount)
{
	Souls += SoulsCount;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.f, MaxStamina);
}

