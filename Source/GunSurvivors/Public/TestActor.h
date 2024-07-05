// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

// Class just used for testing various things
UCLASS()
class GUNSURVIVORS_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

protected:
	// ------------------
	// --- Components --- 
	// ------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class USceneComponent> TestRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestActor|Components")
	TObjectPtr<class UCameraComponent> CameraComp;

	// ----------------------
	// --- Various Values --- 
	// ----------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TestActor|Various Tests")
	TSubclassOf<class AActor> SpawnableActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Various Tests")
	float TestFloatValue = 12345.0f;

	// ----------------------
	// --- Testing Clamps --- 
	// ----------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Clamps", meta = (ClampMin = "0", ClampMax = "1234"))
	float TestClampedFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Clamps", meta = (UIMin = "0", UIMax = "1234"))
	float TestUIClampedFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Clamps", meta = (ClampMin = "0", ClampMax = "1234", UIMin = "0", UIMax = "1234"))
	float TestClampedAndUIClampedFloat;

	// ---------------------
	// --- Testing Units --- 
	// ---------------------
	// NOTE: In order to use Units, they must be listed in the FParseCandidates in UnitConversion.cpp

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Units", meta = (Units = "kg"))
	float TestFloatWithUnit;

	// This is how to use custom units that aren't listed in the ParseCandidates. Don't use these, as there doesn't seem to be any benefit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Units", meta = (ForceUnits = "TestUnit"))
	float TestFloatWithForceUnit;

	// -----------------------------------
	// --- Testing Property Conditions ---
	// -----------------------------------

	// NOTE: For more complex logic on property conditions, use CanEditChange:

	// --- Property Gate ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Gated")
	bool bPropertyGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Gated", meta = (EditCondition = "bPropertyGate"))
	int32 GatedInt;

	// --- Hidden Property Gate ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Hidden Gated", meta = (EditCondition = "bPropertyGate", EditConditionHides))
	int32 GatedHiddenInt;

	// --- Inline Property Gate ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Inline Gated", meta = (InlineEditConditionToggle))
	bool bInlinePropertyGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Inline Gated", meta = (EditCondition = "bInlinePropertyGate"))
	int32 InlineGatedInt;

	// --- Value Property Gate ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Value Gated")
	int32 ValuePropertyGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor|Property Conditions|Value Gated", meta = (EditCondition = "ValuePropertyGate > 15"))
	int32 ValueGatedInt;
};
