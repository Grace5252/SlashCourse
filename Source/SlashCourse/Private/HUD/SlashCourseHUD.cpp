// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashCourseHUD.h"
#include "HUD/SlashCourseOverlay.h"

void ASlashCourseHUD::PreInitializeComponents()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && SlashCourseOverlayClass)
		{
			SlashCourseOverlay = CreateWidget<USlashCourseOverlay>(Controller, SlashCourseOverlayClass);
			SlashCourseOverlay->AddToViewport();
		}
	}
}
