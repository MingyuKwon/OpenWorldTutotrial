// Fill out your copyright notice in the Description page of Project Settings.


#include "TestItem.h"
#include "OpenWorldTutorial/DrawDebugMacro.h"

// Sets default values
ATestItem::ATestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestItem::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MovingVector = FVector(100.f, 0.f, 0.f) * DeltaTime;
	AddActorWorldOffset(FVector(1.f,0.f,0.f));
	DRAW_SPHERE_SINGLEFRAME(GetActorLocation());

}

