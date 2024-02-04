// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestItem.generated.h"

class USphereComponent;

UENUM()
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class OPENWORLDTUTORIAL_API ATestItem : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestItem();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void SphereOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* sphereComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* meshComp;

	EItemState ItemState = EItemState::EIS_Hovering;
	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* EmbersEffect;

private:
	template<typename T>
	T Avg(T first, T second);

	UPROPERTY(VisibleDefaultsOnly)
	float runningTime;
	
};

template<typename T>
inline T ATestItem::Avg(T first, T second)
{
	return (first + second) / 2;
}
