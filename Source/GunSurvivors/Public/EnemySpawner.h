// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();
	void SetupEnemy(class AEnemy* Enemy);

	UFUNCTION()
	void OnEnemyDied();

	UFUNCTION()
	void OnPlayerDied();

protected:
	/** Enemy actor to spawn */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyActorToSpawn;

	/** Time between spawns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;

	/** Distance from this EnemySpawner where enemies will be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 400.0f;

	// --- Difficulty ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DifficultySpikeInterval = 10; // Every 10th enemy, increase difficulty

	/** Time between enemy spawns should go no lower than this value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5;

	/** Descrease the Time between enemy spawns by this value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpawnTimerByEveryInterval = 0.05f; // DifficultySpikeInterval

	// --- Player Reference ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class ATopDownCharacter> Player;

	// --- GameMode Reference --
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AGunSurvivorsGameMode> GunSurvivorsGameMode;

	/** Handle for spawn timer */
	struct FTimerHandle SpawnTimerHandle;
};
