// Copyright 2024 Richard Skala

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

// ----------------------------------------------------------------------------------------------------------------

// --- Enums ---

// You MUST have the BlueprintType specifier in order to use enums in Blueprints
// UENUMs need to be uint8, otherwise will they used end up in the global namespace (source???)
// Since UE 5.4, this is now a compile error (if you don't have the uint8 after the declaration)
// There are multiple range specifiers: https://benui.ca/unreal/iterate-over-enum-tenumrange/
UENUM(BlueprintType) 
enum class ETestEnum : uint8
{
	EnumVal1 UMETA(DisplayName = "Enum Value One!"),
	EnumVal2 UMETA(DisplayName = "val 2"),
	EnumVal3,
	EnumVal4,
	Count UMETA(Hidden) // This cannot be seen or selected in the Blueprint!
};
ENUM_RANGE_BY_COUNT(ETestEnum, ETestEnum::Count); // This is REQUIRED in order to iterate over it in C++ (i.e. TEnumRange)
//ENUM_RANGE_BY_FIRST_AND_LAST(ETestEnum, ETestEnum::EnumVal1, ETestEnum::EnumVal4); => Use this if you don't want a "Count" value
//ENUM_RANGE_BY_VALUES(...) => Use this if you have a non-contiguous range of values

UENUM()
enum class ETestEnum2 // This class cannot be used with Blueprints (
{
	Val1, Val2, Val3
};

// Bitflags / Bitmask test
UENUM(meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETestFlags
{
	CanFly = 1 << 0,
	CanSwim = 1 << 1,
	CanLayEggs = 1 << 2,
	CanPoop = 1 << 3
};
ENUM_CLASS_FLAGS(ETestFlags);

// ----------------------------------------------------------------------------------------------------------------

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

	// -------------------------------------------------------
	// --- Testing Property Conditions using CanEditChange ---
	// -------------------------------------------------------

	// --- Condition with boolean ---

	UPROPERTY(EditAnywhere, Category = "TestActor|Property Conditions|Can Edit Change")
	bool BooleanToggle;

	UPROPERTY(EditAnywhere, Category = "TestActor|Property Conditions|Can Edit Change")
	int32 IntegerProperty;

	// --- Condition with array ---

	UPROPERTY(EditAnywhere, Category = "TestActor|Property Conditions|Can Edit Change")
	TArray<int32> EditConditionArray;

	UPROPERTY(EditAnywhere, Category = "TestActor|Property Conditions|Can Edit Change")
	int32 IntegerProperty2;

	// ---------------------
	// --- Testing Enums ---
	// ---------------------
	UPROPERTY(EditAnywhere, Category = "TestActor|Enums")
	ETestEnum TestEnum;

	UPROPERTY(EditAnywhere, Category = "TestActor|Enums", meta = (Bitmask, BitmaskEnum = ETestFlags))
	int32 TestFlags;
};
