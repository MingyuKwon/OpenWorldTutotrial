// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	RootComponent = capsuleComp;

	skeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeltal Mesh Comp"));
	skeletalComp->SetupAttachment(capsuleComp);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	springArmComp->SetupAttachment(capsuleComp);
	springArmComp->TargetArmLength = 300.f;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	cameraComp->SetupAttachment(springArmComp);

	floatingPawmMovenet = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABird::MoveForward(float value)
{
	if (Controller && value != 0)
	{
		FVector moveVector = GetActorForwardVector();
		AddMovementInput(moveVector, value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
	}
	
}
 
void ABird::Turn(float value)
{
	if (Controller && value != 0)
	{
		AddControllerYawInput(value);
	}
}

void ABird::LookUp(float value)
{
	if (Controller && value != 0)
	{
		AddControllerPitchInput(value * 1.5f);
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this , &ABird::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);

	

}

