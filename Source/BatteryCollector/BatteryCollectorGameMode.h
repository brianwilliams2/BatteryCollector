// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

//enum representing current gamplay state
UENUM(BlueprintType)
enum class EBatteryPlayState:uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime)  override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;
	
	//returns current play state
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;
	//set new play state
	void SetCurrentState(EBatteryPlayState NewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", Meta = (BlueprintProtected = "true"))
		bool playedSound;

protected:
	//rate you lose power
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;
	//used for HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	//instantiate hud
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	private:
		EBatteryPlayState CurrentState;

		TArray<class ASpawnVolume*> SpawnVolumeActors;

		void HandleNewState(EBatteryPlayState NewState);
};



