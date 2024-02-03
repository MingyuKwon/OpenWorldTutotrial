// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayHUD.h"
#include "HUD/OverlayWidget.h"


void AOverlayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController && OverlayClass)
		{
			overlay =  CreateWidget<UOverlayWidget>(playerController, OverlayClass);
			overlay->AddToViewport();
		}
	}

	
}
