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
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "item/Weapon.h"


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

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health bar"));
	HealthBarWidget->SetupAttachment(RootComponent);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::GetHit(const FVector& HitPoint)
{
	//DRAW_SPHERE(HitPoint);
	HealthbarVisible(true);

	if (IsAlive())
	{
		HitReact(HitPoint);
	}
	else
	{
		Die();
	}

	PlayHitSound(HitPoint);
	ShowHitParticle(HitPoint);

}

void AEnemy::Destroyed()
{
	if (obtainWeapon)
	{
		obtainWeapon->Destroy();
	}
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(1.f);
		HealthbarVisible(false);
	}

	AIController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	if (GetWorld() && WeaponClass)
	{
		AWeapon* defaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		defaultWeapon->Equip(this, FName("RightHandSocket"));
		obtainWeapon = defaultWeapon;
	}
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (!AIController || !Target) return;

	FAIMoveRequest moveRequest;
	moveRequest.SetGoalActor(Target);
	moveRequest.SetAcceptanceRadius(60.f);
	AIController->MoveTo(moveRequest);
}

void AEnemy::Die()
{
	// Play Death montage
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	UE_LOG(LogTemp, Display, TEXT("Die"));

	if (DeathMontage && animInstance)
	{
		animInstance->Montage_Play(DeathMontage);

		TArray<FName> dieSecionNames = { FName("Death1"), FName("Death2"), FName("Death3")};
		int8 randIndex = FMath::RandRange(0, 2);
		deathPose = static_cast<EDeathPose>(randIndex + 1);
		FName SectinoName = dieSecionNames[randIndex];
		animInstance->Montage_JumpToSection(SectinoName, DeathMontage);

	}

	HealthbarVisible(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		isInSideAttackRadius() &&
		!IsAttacking() &&
		EnemyState != EEnemyState::EES_Dead;
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(attribute->GetHealthPercent());
	}
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);

}

void AEnemy::PawnSeen(APawn* seenPawn)
{
	const bool bShoudlChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		seenPawn &&
		seenPawn->ActorHasTag(FName("Player"));

	if (bShoudlChaseTarget)
	{
		CombatTarget = seenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	return DamageAmount;
}

bool AEnemy::InTargetInRange(AActor* Target, double radius)
{
	if (Target == nullptr) return false;

	const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return radius >= Distance;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HealthbarVisible(bool isTrue)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(isTrue);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HealthbarVisible(false);
	UE_LOG(LogTemp, Display, TEXT("Loose Interset"));
}

bool AEnemy::isOutSideCombatRadius()
{
	return !InTargetInRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::isOutSideAttackRadius()
{
	return !InTargetInRange(CombatTarget, AttackRadius);
}


bool AEnemy::isInSideAttackRadius()
{
	return InTargetInRange(CombatTarget, AttackRadius);
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}


AActor* AEnemy::ChoosePatrolTarget()
{
	if (PatrolTargets.Num() > 0)
	{
		TArray<AActor*> ValidTargets;
		for (AActor* Target : PatrolTargets)
		{
			if (Target != PatrolTarget)
			{
				ValidTargets.AddUnique(Target);
			}

		}

		int32 randNum = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[randNum];
	}

	return nullptr;
}

void AEnemy::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("Attack"));
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && attackMontage)
	{
		AnimInstance->Montage_Play(attackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, attackMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState > EEnemyState::EES_Dead) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
	
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetInRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f);
	}
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
	UE_LOG(LogTemp, Display, TEXT("Chase"));
}


void AEnemy::CheckCombatTarget()
{
	if (isOutSideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (EnemyState != EEnemyState::EES_Engaged) StartPatrolling();
	}
	else if (isOutSideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (EnemyState != EEnemyState::EES_Engaged) ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

