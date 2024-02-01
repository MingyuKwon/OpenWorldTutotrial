// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "EchoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ATestItem;
class UAnimMontage;


UCLASS()
class OPENWORLDTUTORIAL_API AEchoCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEchoCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(ATestItem* item) {nearItem = item;}
	FORCEINLINE ECharacterState GetCharacterState() const { return characterState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// callback to input
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void EKeyPressed();
	virtual void Attack() override;
	virtual void Jump() override;
	// callback to input

	//Combat
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void PlayEquipMontage(FName SectionName);
	void EquipWeapon(AWeapon* weapon);

	//Combat


	//IHitInterface
	virtual void GetHit(const FVector& HitPoint) override;
	//IHitInterface	

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishingEquipping();

private:
	ECharacterState characterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere)
	ATestItem* nearItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* equipMontage;

};
