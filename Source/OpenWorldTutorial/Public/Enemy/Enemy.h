// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;

UCLASS()
class OPENWORLDTUTORIAL_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& HitPoint) override;

	void HitReact(const FVector& HitPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// montage blueprint function
	UFUNCTION(BlueprintCallable)
	void PlayHitReactMontage(const FName SectionName);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;
	

private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attribute;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* fleshSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticles;

};
