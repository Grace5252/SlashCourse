// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashCourseOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SLASHCOURSE_API USlashCourseOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetCoinCount(int32 Coins);
	void SetSoulCount(int32 Souls);

private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulCountText;
};
