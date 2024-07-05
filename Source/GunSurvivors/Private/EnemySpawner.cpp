// Copyright 2024 Richard Skala

#include "EnemySpawner.h"
//#include "Engine/World.h"
//#include "Engine/TimerHandle.h"
#include "Kismet/GameplayStatics.h"

#include "Enemy.h"
#include "GunSurvivorsGameMode.h"
#include "TopDownCharacter.h"


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

	// Get a reference to the GameMode
	GunSurvivorsGameMode = Cast<AGunSurvivorsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	check(GunSurvivorsGameMode != nullptr);

	// Get player reference
	if (!Player.IsValid())
	{
		UE_LOG(LogEnemySpawner, Log, TEXT("AEnemy::BeginPlay - %s - Player is NOT valid... setting it"), *GetName());

		AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (PlayerActor != nullptr)
		{
			UE_LOG(LogEnemySpawner, Log, TEXT("AEnemy::BeginPlay - %s - Set the player!"), *GetName());
			Player = Cast<ATopDownCharacter>(PlayerActor);
			Player->PlayerDiedDelegate.AddUniqueDynamic(this, &ThisClass::OnPlayerDied);
		}
	}

	// Start spawning
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
		SetupEnemy(SpawnedEnemy);

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

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if (Enemy != nullptr)
	{
		Enemy->SetPlayerTarget(Player.Get());
		Enemy->SetCanFollow(true);
		Enemy->EnemyDiedDelegate.AddDynamic(this, &ThisClass::OnEnemyDied);
	}
}

void AEnemySpawner::OnEnemyDied()
{
	// Increase Score
	const int ScoreToAdd = 10;
	if (GunSurvivorsGameMode != nullptr)
	{
		GunSurvivorsGameMode->AddToScore(10);
	}
}

void AEnemySpawner::OnPlayerDied()
{
	UE_LOG(LogEnemySpawner, Log, TEXT("AEnemySpawner::OnPlayerDied"));

	// Player has died. Stop spawning.
	StopSpawning();

	TArray<AActor*> EnemyActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyActorArray);
	for (AActor* EnemyActor : EnemyActorArray)
	{
		if (AEnemy* Enemy = Cast<AEnemy>(EnemyActor); Enemy->IsAlive())
		{
			Enemy->SetCanFollow(false);
		}
	}

	// TODO: Restart Game
}

