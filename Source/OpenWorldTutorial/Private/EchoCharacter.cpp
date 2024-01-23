// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AEchoCharacter::AEchoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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
	
}

void AEchoCharacter::MoveForward(float value)
{
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
}

