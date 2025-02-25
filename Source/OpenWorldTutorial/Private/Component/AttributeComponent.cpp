// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health -= Damage;
	Health = FMath::Clamp(Health, 0, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return  Health/MaxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

void UAttributeComponent::AddSouls(int32 soulCount)
{
	Soul += soulCount;
}

void UAttributeComponent::AddGold(int32 goldCount)
{
	Gold += goldCount;
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

