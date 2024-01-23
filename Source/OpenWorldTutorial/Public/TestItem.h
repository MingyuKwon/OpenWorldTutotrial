// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestItem.generated.h"

UCLASS()
class OPENWORLDTUTORIAL_API ATestItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestItem();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	template<typename T>
	T Avg(T first, T second);

};

template<typename T>
inline T ATestItem::Avg(T first, T second)
{
	return (first + second) / 2;
}
