// Copyright 2024 Richard Skala


#include "GunSurvivorsGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogGunSurvivorsGameMode, Log, All)

AGunSurvivorsGameMode::AGunSurvivorsGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	SetScore(0);
}

void AGunSurvivorsGameMode::SetScore(int NewScore)
{
	if (NewScore < 0)
	{
		return;
	}
	Score = NewScore;
	
	ScoreChangedDelegate.Broadcast(Score);
}

void AGunSurvivorsGameMode::AddToScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Score: %d"), Score));
}

void AGunSurvivorsGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetScore(0);
}
