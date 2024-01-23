// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class UFloatingPawnMovement;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class OPENWORLDTUTORIAL_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsuleComp;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* skeletalComp;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* cameraComp;

	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* floatingPawmMovenet;

private:
	void MoveForward(float value);
	void Turn(float value);
	void LookUp(float value);

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 10.f;
};
