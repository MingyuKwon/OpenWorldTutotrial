// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestItem.h"
#include "Weapon.generated.h"

/**
 * 
 */
class UBoxComponent;



UCLASS()
class OPENWORLDTUTORIAL_API AWeapon : public ATestItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(ACharacter* InParent, FName InSocketName);

	void AttachActortoSocket(ACharacter* InParent, const FName& InSocketName);

	void EnableWeaponAttackCollision(bool isTrue);
	
protected:
	virtual void BeginPlay() override;

	virtual void SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void SphereOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void BoxOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(EditAnywhere, Category = "Weapon Category")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Category")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* BoxTraceStart;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* BoxTraceEnd;

	TArray<AActor*> IgnoreActors;


	UPROPERTY(EditDefaultsOnly)
	float Damage = 30.f;
};
