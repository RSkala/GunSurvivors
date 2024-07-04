// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GunSurvivorsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AGunSurvivorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGunSurvivorsGameMode();
	int GetScore() const { return Score; }
	void SetScore(int NewScore);
	void AddToScore(int AmountToAdd);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Score = 0;
};
