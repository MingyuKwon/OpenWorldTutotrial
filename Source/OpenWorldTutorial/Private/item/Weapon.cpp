// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Weapon.h"
#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(RootComponent);

	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(RootComponent);

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(RootComponent);
}

void AWeapon::Equip(ACharacter* InParent, FName InSocketName)
{
	AttachActortoSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());

	}

	if (sphereComp)
	{
		sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachActortoSocket(ACharacter* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules transformRule(EAttachmentRule::SnapToTarget, true);
	meshComp->AttachToComponent(InParent->GetMesh(), transformRule, InSocketName);
}

void AWeapon::EnableWeaponAttackCollision(bool isTrue)
{
	if (!WeaponBox) return;

	IgnoreActors.Empty();
	if (isTrue)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BoxOverlapBeginCallback);

}

void AWeapon::SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereOverlapBeginCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::SphereOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::SphereOverlapEndCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeapon::BoxOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,1.f,FColor::Emerald, TEXT("Overlap Start"));
	}

	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (AActor* actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(actor);
	}

	FHitResult hitResult;
	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(), 
		Start, 
		End, 
		FVector(5.f,5.f,5.f), 
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true
		);

	if (hitResult.GetActor())
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
		if (hitInterface)
		{
			hitInterface->GetHit(hitResult.ImpactPoint);
		}
		IgnoreActors.AddUnique(hitResult.GetActor());
	}

}
