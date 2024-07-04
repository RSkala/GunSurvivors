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

	// Spawn the enemy

	// Get a random position WITHIN the unit sphere. Cast to FVector2D.
	FVector2D RandomPosition = FVector2D(FMath::VRand());

	// Normalize the random position to "push" the position to the edge of the unit circle
	RandomPosition.Normalize();

	// Multiply by the spawn distance for the spawn position on the circle
	RandomPosition *= SpawnDistance;

	// Spawn the enemy at the location
	if (UWorld* World = GetWorld())
	{
		FVector EnemyLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);
		AEnemy* SpawnedEnemy = World->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, FRotator::ZeroRotator);

		// Increase the difficulty
		TotalEnemyCount += 1;
		if (TotalEnemyCount % DifficultySpikeInterval == 0)
		{
			// Ensure we haven't gone past the difficulty (spawn time) limit
			if (SpawnTime > SpawnTimeMinimumLimit)
			{
				// Ensure we dont go below spawn time
				SpawnTime -= DecreaseSpawnTimerByEveryInterval; // should be "increase"
				if (SpawnTime < SpawnTimeMinimumLimit)
				{
					SpawnTime = SpawnTimeMinimumLimit;
				}

				// Stop and restart the spawning using the new SpawnTime
				StopSpawning();
				StartSpawning();
			}
		}
	}
}

