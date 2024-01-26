// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Weapon.h"
#include "EchoCharacter.h"

void AWeapon::Equip(ACharacter* InParent, FName InSocketName)
{
	FAttachmentTransformRules transformRule(EAttachmentRule::SnapToTarget, true);
	meshComp->AttachToComponent(InParent->GetMesh(), transformRule, InSocketName);
	ItemState = EItemState::EIS_Equipped;
}

void AWeapon::ComponentOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::ComponentOverlapBeginCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::ComponentOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ComponentOverlapEndCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}
