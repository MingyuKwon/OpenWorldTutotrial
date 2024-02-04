// Fill out your copyright notice in the Description page of Project Settings.


#include "TestItem.h"
#include "OpenWorldTutorial/DrawDebugMacro.h"
#include "Components/SphereComponent.h"
#include "PickupInterface.h"
#include "NiagaraComponent.h"

// Sets default values
ATestItem::ATestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = meshComp;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	sphereComp->SetupAttachment(RootComponent);

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ember"));
	EmbersEffect->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATestItem::BeginPlay()
{
	Super::BeginPlay();	
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATestItem::SphereOverlapBeginCallback);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &ATestItem::SphereOverlapEndCallback);
	runningTime = 0;
}



void ATestItem::SphereOverlapBeginCallback( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* hitInterface = Cast<IPickupInterface>(OtherActor);
	if (hitInterface)
	{
		hitInterface->SetOverlappingItem(this);
	}
	
}

void ATestItem::SphereOverlapEndCallback( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* hitInterface = Cast<IPickupInterface>(OtherActor);	
	if (hitInterface)
	{
		hitInterface->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void ATestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		float SinValue = FMathf::Sin(runningTime) / 3;
		FVector MovingVector = FVector(0.f, 0.f, SinValue) * 100 * DeltaTime;
		AddActorWorldOffset(MovingVector);
	}
}

