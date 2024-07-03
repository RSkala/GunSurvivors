// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector2D InMovementDirection, float InMovementSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDeleteTimerTimeout();

protected:
	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> BulletSpriteComp;

	// --- Movement ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLaunched = false;

	struct FTimerHandle DeleteTimer;

};
