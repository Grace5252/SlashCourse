// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashCourseHUD.generated.h"

class USlashCourseOverlay;

UCLASS()
class SLASHCOURSE_API ASlashCourseHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "SlashCourse")
	TSubclassOf<USlashCourseOverlay> SlashCourseOverlayClass;

	UPROPERTY()
	USlashCourseOverlay* SlashCourseOverlay;
public:
	FORCEINLINE USlashCourseOverlay* GetSlashCourseOverlay() const { return SlashCourseOverlay; }
};
