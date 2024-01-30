// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"


void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (healthBarWidget == nullptr)
	{
		healthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if(healthBarWidget && healthBarWidget->healthBar)
	{
		healthBarWidget->healthBar->SetPercent(Percent);
	}
}
