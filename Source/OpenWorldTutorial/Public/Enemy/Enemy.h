// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterType.h"
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

	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	// montage blueprint function
	UFUNCTION(BlueprintCallable)
	void PlayHitReactMontage(const FName SectionName);

	void Die();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose = EDeathPose::EDP_Alive;

	bool InTargetInRange(AActor* Target, double radius);
	

private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attribute;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* fleshSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticles;

	UPROPERTY();
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere);
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere);
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	// Navigarion
	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY();
	class AAIController* AIController;
	

};
