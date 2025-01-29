// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashCourseOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashCourseOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void USlashCourseOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void USlashCourseOverlay::SetCoinCount(int32 Coins)
{
	if (CoinCountText)
	{
		CoinCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coins)));
	}
}

void USlashCourseOverlay::SetSoulCount(int32 Souls)
{
	if (SoulCountText)
	{
		SoulCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
