// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "OpenWorldTutorial/DrawDebugMacro.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Component/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);

	attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health bar"));
	HealthBarWidget->SetupAttachment(RootComponent);
}

void AEnemy::GetHit(const FVector& HitPoint)
{
	//DRAW_SPHERE(HitPoint);
	
	if (attribute && attribute->IsAlive())
	{
		HitReact(HitPoint);

		if (fleshSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), fleshSound, HitPoint);
		}

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitPoint);
		}
	}
	else
	{
		Die();
	}

	
}

void AEnemy::HitReact(const FVector& HitPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ToHit = (HitPoint - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	FName Section("Back");

	if (Theta >= -45.f && Theta <= 45.f)
	{
		Section = FName("Forward");
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Forward - Theta: %f"), Theta));
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("Left");
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Black, FString::Printf(TEXT("Left - Theta: %f"), Theta));
	}
	else if (Theta <= 135.f && Theta > 45.f)
	{
		Section = FName("Right");
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, FString::Printf(TEXT("Right - Theta: %f"), Theta));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("Back - Theta: %f"), Theta));
	}

	PlayHitReactMontage(Section);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(1.f);
	}
}

void AEnemy::PlayHitReactMontage(const FName SectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (HitReactMontage && animInstance)
	{
		animInstance->Montage_Play(HitReactMontage);
		animInstance->Montage_JumpToSection(SectionName, HitReactMontage);

	}
}

void AEnemy::Die()
{
	// Play Death montage
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (DeathMontage && animInstance)
	{
		animInstance->Montage_Play(DeathMontage);

		TArray<FName> dieSecionNames = { FName("Death1"), FName("Death2"), FName("Death3")};
		int8 randIndex = FMath::RandRange(0, 2);
		deathPose = static_cast<EDeathPose>(randIndex + 1);
		FName SectinoName = dieSecionNames[randIndex];
		animInstance->Montage_JumpToSection(SectinoName, DeathMontage);

	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attribute && HealthBarWidget)
	{
		attribute->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(attribute->GetHealthPercent());
	}

	return DamageAmount;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

