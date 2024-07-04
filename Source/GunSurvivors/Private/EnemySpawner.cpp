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
	}
}

