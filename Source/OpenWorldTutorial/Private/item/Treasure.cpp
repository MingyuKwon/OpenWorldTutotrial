// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Treasure.h"
#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"

int32 ATreasure::GetGoldValue()
{
	return Gold;
}

void ATreasure::SphereOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEchoCharacter* character = Cast<AEchoCharacter>(OtherActor);
	if (character)
	{
		if (coinSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), coinSound, GetActorLocation());
		}
		Destroy();
	}
}

void ATreasure::SphereOverlapEndCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
