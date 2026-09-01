#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "SpartaGameInstance.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;

	FWaveData Wave1;
	Wave1.Duration = 10.0f;
	Wave1.ItemCount = 10;

	WaveDataList.Add(Wave1);

	FWaveData Wave2;
	Wave2.Duration = 5.0f;
	Wave2.ItemCount = 15;

	WaveDataList.Add(Wave2);

	FWaveData Wave3;
	Wave3.Duration = 3.0f;
	Wave3.ItemCount = 20;

	WaveDataList.Add(Wave3);
}


void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentWaveIndex = 0;

	StartWave();

	
}

void ASpartaGameState::StartWave()
{
	if (!WaveDataList.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}
	FWaveData& CurrentWave = WaveDataList[CurrentWaveIndex];

	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작! Duration: %.1f"), CurrentWaveIndex + 1, CurrentWave.Duration);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Wave %d 시작!"), CurrentWaveIndex + 1)
		);
	}

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ASpartaGameState::OnWaveTimeup,
		CurrentWave.Duration,
		false
	);

	TArray<AActor*>FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawnVolume::StaticClass(),
		FoundVolumes
	);

	for (int32 i = 0; i < CurrentWave.ItemCount; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);

			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

				if (SpawnedActor && SpawnedActor->IsA(ACoinitem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
}

void ASpartaGameState::OnWaveTimeup()
{
	CurrentWaveIndex++;

	if (WaveDataList.IsValidIndex(CurrentWaveIndex))
	{
		StartWave();
	}
	else
	{
		EndLevel();
	}
}


void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount)
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}
void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
	
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
					
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}


				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveText->SetText(
						FText::FromString(
							FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)
						)
					);
				}
			}
		}
	}

}