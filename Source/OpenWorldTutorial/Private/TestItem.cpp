// Fill out your copyright notice in the Description page of Project Settings.


#include "TestItem.h"
#include "OpenWorldTutorial/DrawDebugMacro.h"
#include "Components/SphereComponent.h"

// Sets default values
ATestItem::ATestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = meshComp;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	sphereComp->SetupAttachment(RootComponent);

	

}

// Called when the game starts or when spawned
void ATestItem::BeginPlay()
{
	Super::BeginPlay();	
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATestItem::ComponentOverlapBeginCallback);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATestItem::ComponentOverlapEndCallback);
	runningTime = 0;
}



void ATestItem::ComponentOverlapBeginCallback( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Blue, OtherActor->GetName());
	}
}

void ATestItem::ComponentOverlapEndCallback( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, OtherActor->GetName());
	}
}

// Called every frame
void ATestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningTime += DeltaTime;

	if (!GetOwner())
	{
		float SinValue = FMathf::Sin(runningTime) / 3;
		FVector MovingVector = FVector(0.f, 0.f, SinValue) * 50 * DeltaTime;
		AddActorWorldOffset(MovingVector);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Yellow, GetOwner()->GetName());
		}
	}
	

}

