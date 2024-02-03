// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UOverlayWidget::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UOverlayWidget::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UOverlayWidget::SetGold(int32 count)
{
	if (coinAmount)
	{
		const FString String = FString::Printf(TEXT("%d"), count);
		const FText Text = FText::FromString(String);
		coinAmount->SetText(Text);
	}
}

void UOverlayWidget::SetSoul(int32 count)
{
	if (soulAmount)
	{
		const FString String = FString::Printf(TEXT("%d"), count);
		const FText Text = FText::FromString(String);
		soulAmount->SetText(Text);
	}
}
