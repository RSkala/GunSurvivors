// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopDownCharacter.generated.h"

UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopDownCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -- Movement --
	void MoveTriggered(const struct FInputActionValue& InputActionValue);
	void MoveCompleted(const struct FInputActionValue& InputActionValue);

	// --- Shooting ---
	void Shoot(const struct FInputActionValue& InputActionValue);
	void OnShootCooldownTimerTimeout();

	// --- Map Bounds ---
	bool IsInMapBoundsHorizontal(float XPos) const;
	bool IsInMapBoundsVertical(float ZPos) const;

protected:
	//  --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperFlipbookComponent> CharacterFlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> DummyGunComp; // This is a workaround for not being able to set GunParentComp's Rotation and Location in the Blueprint Editor

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> GunParentComp; // This should be a child of Dummy

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPaperSpriteComponent> GunSpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> BulletSpawnPosition;

	//  --- Input ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TopDownCharacter|Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TopDownCharacter|Input")
	TObjectPtr<class UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TopDownCharacter|Input")
	TObjectPtr<class UInputAction> ShootInputAction;

	//  --- Flipbook ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TopDownCharacter|Flipbook")
	TObjectPtr<class UPaperFlipbook> IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TopDownCharacter|Flipbook")
	TObjectPtr<class UPaperFlipbook> RunFlipbook;

	//  --- Map Limits ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Map Limits")
	FVector2D HorizontalLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Map Limits")
	FVector2D VerticalLimits;

	// --- Movement ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Movement")
	float MovementSpeed = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Movement")
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Movement")
	bool bCanMove = true;

	// --- Shooting --- 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Shooting")
	bool bCanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TopDownCharacter|Shooting")
	float ShootCooldownDurationSeconds = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "TopDownCharacter|Shooting")
	TSubclassOf<class ABullet> BulletActorToSpawn;

	struct FTimerHandle ShootCooldownTimerHandle;
};
