// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;

UCLASS()
class OPENWORLDTUTORIAL_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	//components
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attribute;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* fleshSound;

	UFUNCTION(BlueprintCallable)
	void EnableObtainWeaponAttackCollision(bool isTrue);

	virtual void PlayHitSound(const FVector& Location);

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticles;

	virtual void ShowHitParticle(const FVector& Location);

	virtual void HandleDamage(float DamageAmount);
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* obtainWeapon;

	virtual void Attack();

	virtual void Die();

	virtual bool IsAlive();

	virtual void DisableCapsule();

	// playa montage function
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();

	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// montage blueprint function
	UFUNCTION(BlueprintCallable)
	void PlayHitReactMontage(const FName SectionName);

	virtual void GetHit(const FVector& HitPoint);

	virtual void HitReact(const FVector& HitPoint);

private:
	void PlayMontageSection(UAnimMontage* montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* montage, const TArray<FName>& SectionNames);

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* attackMontage;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat");
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat");
	TArray<FName> DeathMontageSections;
};
