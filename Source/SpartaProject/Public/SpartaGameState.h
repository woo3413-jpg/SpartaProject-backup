#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpartaGameState.generated.h"

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float Duration = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ItemCount= 10;


};


UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameStateBase
{
	GENERATED_BODY()


public:
	ASpartaGameState();

	virtual void BeginPlay()override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FWaveData> WaveDataList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Wave")
	int32 CurrentWaveIndex;


	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void StartWave();
	void OnWaveTimeup();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
};
