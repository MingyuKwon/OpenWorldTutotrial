// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestItem.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDTUTORIAL_API ASoul : public ATestItem
{
	GENERATED_BODY()
	
protected:
	virtual void SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	
};
