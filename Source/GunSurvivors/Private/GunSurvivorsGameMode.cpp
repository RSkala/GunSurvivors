// Copyright 2024 Richard Skala


#include "GunSurvivorsGameMode.h"

#include "Kismet/GameplayStatics.h"

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

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Score: %d"), Score));
}

void AGunSurvivorsGameMode::RestartGame()
{
	UE_LOG(LogGunSurvivorsGameMode, Log, TEXT("AGunSurvivorsGameMode::RestartGame"));
	GetWorldTimerManager().SetTimer(RestartGameTimerHandle, this, &ThisClass::OnRestartGameTimerTimeout, 1.0f, false, TimeBeforeRestart);
}

void AGunSurvivorsGameMode::OnRestartGameTimerTimeout()
{
	UE_LOG(LogGunSurvivorsGameMode, Log, TEXT("AGunSurvivorsGameMode::RestartGame"));

	GetWorldTimerManager().ClearTimer(RestartGameTimerHandle);

	// Reload the same level
	//UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetMapName()));
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void AGunSurvivorsGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetScore(0);
}
