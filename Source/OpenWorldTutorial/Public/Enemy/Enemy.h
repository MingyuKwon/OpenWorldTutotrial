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
	AEnemy();

	//State
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	//State

	// AActor
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void Destroyed() override;
	// AActor

protected:
	// AActor
	virtual void BeginPlay() override;
	// AActor

	// ABaseCharacter
	virtual void Die() override;

	virtual void Attack() override;

	virtual bool CanAttack() override;

	virtual void HandleDamage(float DamageAmount) override;

	virtual int32 PlayDeathMontage() override;

	virtual void AttackEnd() override;
	// ABaseCharacter
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose;

private:

	//AI Behavior
	void CheckPatrolTarget();
	void CheckCombatTarget();

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY();
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere);
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere);
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere);
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "Ai Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY();
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, Category = "Moving")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "Moving")
	float ChasingSpeed = 300.f;

	void HealthbarVisible(bool isTrue);
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool isOutSideCombatRadius();
	bool isOutSideAttackRadius();
	bool isInSideAttackRadius();
	bool IsChasing();
	bool IsAttacking();

	void StartAttackTimer();
	void ClearAttackTimer();
	void ClearPatrolTimer();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackTime = 0.5f;

	UFUNCTION()
	void PawnSeen(APawn* seenPawn); // Call back for OnPawn Seen in SensingComponent

	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	bool InTargetInRange(AActor* Target, double radius);
	//AI Behavior


	//IHitInterface
	virtual void GetHit(const FVector& HitPoint, AActor* Hitter) override;
	//IHitInterface	

	
};
