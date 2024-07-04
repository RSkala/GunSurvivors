// Copyright 2024 Richard Skala

#include "EnemySpawner.h"
//#include "Engine/World.h"
//#include "Engine/TimerHandle.h"

#include "Enemy.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemySpawner, Log, All)

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	StartSpawning();
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

void AEnemySpawner::SpawnEnemy()
{
	UE_LOG(LogEnemySpawner, Log, TEXT("AEnemySpawner::SpawnEnemy"));

	if (UWorld* World = GetWorld())
	{
		FVector EnemyLocation = GetActorLocation();
		AEnemy* SpawnedEnemy = World->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, FRotator::ZeroRotator);
	}
}

