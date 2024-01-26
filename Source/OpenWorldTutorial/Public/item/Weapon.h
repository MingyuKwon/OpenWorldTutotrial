// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestItem.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDTUTORIAL_API AWeapon : public ATestItem
{
	GENERATED_BODY()
public:
	void Equip(ACharacter* InParent, FName InSocketName);

	void AttachActortoSocket(ACharacter* InParent, const FName& InSocketName);
	
protected:

	virtual void ComponentOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void ComponentOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
