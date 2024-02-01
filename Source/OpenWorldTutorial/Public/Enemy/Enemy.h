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

	virtual void GetHit(const FVector& HitPoint) override;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	virtual void Attack() override;

	virtual void Die() override;
	virtual bool CanAttack() override;

	virtual void HandleDamage(float DamageAmount) override;

	UFUNCTION()
	void PawnSeen(APawn* seenPawn);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose;

	bool InTargetInRange(AActor* Target, double radius);
	
	virtual int32 PlayDeathMontage() override;

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


	// AI Functions
	void HealthbarVisible(bool isTrue);
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool isOutSideCombatRadius();
	bool isOutSideAttackRadius();
	bool isInSideAttackRadius();
	bool IsChasing();
	bool IsAttacking();

	UPROPERTY(EditAnywhere, Category = "Moving")
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = "Moving")
	float ChasingSpeed = 300.f;

	// Combat
	void StartAttackTimer();
	void ClearAttackTimer();
	void ClearPatrolTimer();
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTime = 0.5f;
	
	
};
