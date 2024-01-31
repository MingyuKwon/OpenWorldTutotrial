// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class OPENWORLDTUTORIAL_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& HitPoint) override;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	virtual void Attack() override;
	virtual void PlayAttackMontage() override;

	virtual void Die() override;

	UFUNCTION()
	void PawnSeen(APawn* seenPawn);

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
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY();
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere);
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere);
	double AttackRadius = 150.f;

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
