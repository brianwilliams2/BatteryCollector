// Fill out your copyright notice in the Description page of Project Settings.

#include "WinTrigger.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

AWinTrigger::AWinTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &AWinTrigger::OnBoxTrigger);
}

void AWinTrigger::BeginPlay()
{
	Super::BeginPlay();


}

void AWinTrigger::OnBoxTrigger(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

		if (MyCharacter)
		{
			MyCharacter->WinTriggered = true;
		}
	}
}