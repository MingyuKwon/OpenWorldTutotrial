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


	UFUNCTION(BlueprintCallable)
	void EnableObtainWeaponAttackCollision(bool isTrue);

protected:
	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* attackMontage;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	//components
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attribute;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* fleshSound;

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

	// playa montage function
	virtual void PlayAttackMontage();

	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// montage blueprint function
	UFUNCTION(BlueprintCallable)
	void PlayHitReactMontage(const FName SectionName);

	virtual void GetHit(const FVector& HitPoint) override;

	virtual void HitReact(const FVector& HitPoint);


};
