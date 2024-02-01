// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "item/Weapon.h"
#include "Component/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"



ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABaseCharacter::PlayHitSound(const FVector& Location)
{
	if (fleshSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), fleshSound, Location);
	}
}

void ABaseCharacter::ShowHitParticle(const FVector& Location)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Location);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (attribute)
	{
		attribute->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && montage)
	{
		AnimInstance->Montage_Play(montage);
		AnimInstance->Montage_JumpToSection(SectionName, montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(montage, SectionNames[Selection]);

	return Selection;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

bool ABaseCharacter::IsAlive()
{
	return attribute && attribute->IsAlive();
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(attackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName SectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (HitReactMontage && animInstance)
	{
		animInstance->Montage_Play(HitReactMontage);
		animInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::GetHit(const FVector& HitPoint)
{

}

void ABaseCharacter::HitReact(const FVector& HitPoint)
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

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::EnableObtainWeaponAttackCollision(bool isTrue)
{
	if (obtainWeapon)
	{
		obtainWeapon->EnableWeaponAttackCollision(isTrue);
	}
}



