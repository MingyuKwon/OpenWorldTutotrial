// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestItem.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDTUTORIAL_API ATreasure : public ATestItem
{
	GENERATED_BODY()

public:
	int32 GetGoldValue();

protected:
	virtual void SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SphereOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* coinSound;

	UPROPERTY(EditDefaultsOnly, Category = "Treasure property")
	int32 Gold;
};
