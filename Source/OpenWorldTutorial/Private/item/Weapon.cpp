// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Weapon.h"
#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"
#include "NiagaraComponent.h"


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
	PlayEquipSound();
	DisableSphereCollision();
	DeactiveEmberEffect();
}

void AWeapon::DeactiveEmberEffect()
{
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}

void AWeapon::DisableSphereCollision()
{
	if (sphereComp)
	{
		sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
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

void AWeapon::BoxOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsHasSameTag(OtherActor)) return;

	FHitResult hitResult;
	BoxTrace(hitResult);

	if (hitResult.GetActor())
	{
		if (IsHasSameTag(hitResult.GetActor())) return;

		UGameplayStatics::ApplyDamage(
			hitResult.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		ExecuteGetHit(hitResult);
	}
}

bool AWeapon::IsHasSameTag(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Enemy");
}

void AWeapon::ExecuteGetHit(FHitResult& hitResult)
{
	IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
	if (hitInterface)
	{
		hitInterface->GetHit(hitResult.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& HitResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (AActor* actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);
	IgnoreActors.AddUnique(HitResult.GetActor());
}
