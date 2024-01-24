// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoAnimInstance.h"
#include "EchoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
	if (EchoCharacter)
	{
		EchoCharacterMovement =  EchoCharacter->GetCharacterMovement();
	}
	

}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (EchoCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(EchoCharacterMovement->Velocity);
		isFalling = EchoCharacterMovement->IsFalling();
	}

}
