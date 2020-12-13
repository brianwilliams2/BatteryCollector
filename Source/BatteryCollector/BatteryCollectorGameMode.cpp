// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "BatteryCollectorGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	DecayRate = 0.01f;
	PrimaryActorTick.bCanEverTick = true;

}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	playedSound = false;

	//find all spawn actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower()) * 1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//check if we are using character
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		// If the character has touched the win trigger
		if (MyCharacter->WinTriggered)
		{
		// Win
			SetCurrentState(EBatteryPlayState::EWon);
			if (!playedSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), MyCharacter->WinSound, MyCharacter->GetActorLocation(), 2.0f, 1.0f, 0.0f);
				playedSound = true;
			}
		}
		// If your gas tank is over full
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			// The difference between the player's current gas amount and the amount of a full tank of gas
			float overflowPower = MyCharacter->GetCurrentPower() - PowerToWin;
			// Cap gas amount
			MyCharacter->UpdatePower(-overflowPower);
		}
		if (MyCharacter->GetCurrentPower() > 0 && !MyCharacter->WinTriggered)
		{
			//decrease power by decay rate
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
		//lose condition
		if (MyCharacter->GetCurrentPower() <= 0)
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
			if (!playedSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), MyCharacter->LoseSound, MyCharacter->GetActorLocation(), 2.0f, 1.0f, 0.0f);
				playedSound = true;
			}
		}

		// Now the player will automatically collect any pickups they run into, rather than having to press a key
		MyCharacter->CollectPickups();
		MyCharacter->ObstaclePickups();
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const 
{
	return PowerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
	case EBatteryPlayState::EPlaying:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(true);
		}
	}
	break;
	case EBatteryPlayState::EWon:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
		//lock player movement
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
	}
	break;
	case EBatteryPlayState::EGameOver:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
		//lock player movement
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		/*ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}*/
	}
	break;
	case EBatteryPlayState::EUnknown:
	{

	}
	break;
	}
}