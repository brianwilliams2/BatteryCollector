// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "WinTrigger.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API AWinTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AWinTrigger();

	UFUNCTION()
		void OnBoxTrigger(class AActor* overlappedActor, class AActor* otherActor);
};
