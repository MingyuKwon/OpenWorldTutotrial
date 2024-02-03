// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OverlayHUD.generated.h"

/**
 * 
 */

class UOverlayWidget;

UCLASS()
class OPENWORLDTUTORIAL_API AOverlayHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlayWidget> OverlayClass;

	UOverlayWidget* overlay;

public:
	FORCEINLINE UOverlayWidget* GetWidget() const { return overlay;}
};
