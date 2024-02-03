// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestItem.h"
#include "item/Weapon.h"
#include "Animation/AnimMontage.h"



// Sets default values
AEchoCharacter::AEchoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);  
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = 200.f;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	cameraComp->SetupAttachment(springArmComp);
}

// Called when the game starts or when spawned
void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngagableTarget"));
}

void AEchoCharacter::MoveForward(float value)
{
	if (ActionState == EActionState::EAS_Attacking || ActionState == EActionState::EAS_EquippingWeapon) return;
	if (Controller && value != 0)
	{
		const FRotator ControlRotaion = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotaion.Yaw, 0.f);

		const FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
	}
}

void AEchoCharacter::MoveRight(float value)
{
	if (ActionState == EActionState::EAS_Attacking || ActionState == EActionState::EAS_EquippingWeapon) return;
	if (Controller && value != 0)
	{
		const FRotator ControlRotaion = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotaion.Yaw, 0.f);

		const FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));


	}
}

void AEchoCharacter::Turn(float value)
{
	if (Controller && value != 0)
	{
		AddControllerYawInput(value);
	}
}

void AEchoCharacter::LookUp(float value)
{
	if (Controller && value != 0)
	{
		AddControllerPitchInput(value * 1.5f);
	}
}

void AEchoCharacter::Jump()
{
	if (Controller)
	{
		Super::Jump();
	}
}

void AEchoCharacter::EKeyPressed()
{
	AWeapon* weapon = Cast<AWeapon>(nearItem);
	if (weapon)
	{
		EquipWeapon(weapon);
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(TEXT("UnEquip"));
			characterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipMontage(TEXT("Equip"));
			characterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void AEchoCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AEchoCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && equipMontage)
	{
		AnimInstance->Montage_Play(equipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, equipMontage);
	}
}

void AEchoCharacter::EquipWeapon(AWeapon* weapon)
{
	weapon->Equip(this, TEXT("right hand socket"));
	weapon->SetOwner(this);
	weapon->SetInstigator(this);
	characterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	nearItem = nullptr;
	obtainWeapon = weapon;
}

float AEchoCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;
}

void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AEchoCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied 
		&& characterState != ECharacterState::ECS_Unequipped;
}

bool AEchoCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		characterState != ECharacterState::ECS_Unequipped && 
		equipMontage && obtainWeapon;
}

bool AEchoCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		characterState == ECharacterState::ECS_Unequipped &&
		equipMontage && obtainWeapon;
}

void AEchoCharacter::GetHit(const FVector& HitPoint, AActor* Hitter)
{
	Super::GetHit(HitPoint, Hitter);
	EnableObtainWeaponAttackCollision(false);
}

void AEchoCharacter::Disarm()
{
	if (obtainWeapon)
	{
		obtainWeapon->AttachActortoSocket(this, TEXT("SwordCase Socket"));
		characterState = ECharacterState::ECS_Unequipped;
	}
}

void AEchoCharacter::Arm()
{
	if (obtainWeapon)
	{
		obtainWeapon->AttachActortoSocket(this, TEXT("right hand socket"));
		characterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void AEchoCharacter::FinishingEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}


// Called every frame
void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEchoCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEchoCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEchoCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEchoCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed , this, &AEchoCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), EInputEvent::IE_Pressed, this, &AEchoCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AEchoCharacter::Attack);

}

