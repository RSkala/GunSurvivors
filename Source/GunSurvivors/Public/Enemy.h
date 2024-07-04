// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsAlive() const { return bIsAlive; }

	void Die();
	void SetPlayerTarget(class ATopDownCharacter* InPlayer); // Cannot assign here as this is a foward declaration. Must do the assignment in the cpp file.
	void SetCanFollow(bool bInCanFollow) { bCanFollow = bInCanFollow; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDestroyTimerTimeout();

protected:
	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> FlipbookComp;

	// --- Flipbook Assets ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPaperFlipbook> DeadFlipbookAsset;

	// --- Player Reference ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class ATopDownCharacter> Player;

	// --- Lifetime / Movement ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;

	struct FTimerHandle DestroyTimerHandle;
};
