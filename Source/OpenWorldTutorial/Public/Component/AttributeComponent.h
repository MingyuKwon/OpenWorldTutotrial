// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDTUTORIAL_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

private:
	//Current Health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	int32 Soul;
		
public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();
	void AddSouls(int32 soulCount);
	void AddGold(int32 goldCount);

	FORCEINLINE int32 GetGold() { return Gold; };
	FORCEINLINE int32 GetSoul() { return Soul; };
};
