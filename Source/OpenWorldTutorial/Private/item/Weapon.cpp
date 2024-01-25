// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Weapon.h"
#include "EchoCharacter.h"

void AWeapon::ComponentOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::ComponentOverlapBeginCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AEchoCharacter* character = Cast<AEchoCharacter>(OtherActor);
	if (character)
	{
		FAttachmentTransformRules transformRule(EAttachmentRule::SnapToTarget,true);
		meshComp->AttachToComponent(character->GetMesh(), transformRule, TEXT("right hand socket"));
		this->SetOwner(OtherActor);
	}
}

void AWeapon::ComponentOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ComponentOverlapEndCallback(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}
