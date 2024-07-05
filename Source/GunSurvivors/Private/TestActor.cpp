// Copyright 2024 Richard Skala


#include "TestActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTestActor, Log, All)

// Sets default values
ATestActor::ATestActor()
{
	UE_LOG(LogTestActor, Log, TEXT("ATestActor::ATestActor - %s"), *GetName());

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	TestRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("TestRootComp"));
	SetRootComponent(TestRootComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	UE_LOG(LogTestActor, Log, TEXT("ATestActor::BeginPlay - %s"), *GetName());

	Super::BeginPlay();
}

#if WITH_EDITOR
bool ATestActor::CanEditChange(const FProperty* InProperty) const
{
	FString PropertyNameString = InProperty != nullptr ? InProperty->GetName() : "(invalid)";
	UE_LOG(LogTestActor, Log, TEXT("ATestActor::CanEditChange - Property: %s"), *PropertyNameString);

	// Get the name of the property being edited
	const FName InPropertyName = InProperty != nullptr ? InProperty->GetFName() : NAME_None;

	// Boolean test
	const FName IntegerPropertyName = GET_MEMBER_NAME_CHECKED(ATestActor, IntegerProperty);
	if (InPropertyName == IntegerPropertyName)
	{
		return BooleanToggle;
	}

	// Array test
	const FName IntegerProperty2Name = GET_MEMBER_NAME_CHECKED(ATestActor, IntegerProperty2);
	if (InPropertyName == IntegerProperty2Name)
	{
		// Example -- If sum of array > 10, then we can edit the property
		int32 Sum = 0;
		for (const int32& CurrentValue : EditConditionArray)
		{
			Sum += CurrentValue;
		}
		return Sum > 10;
	}

	// Edit Conditions for other Properties

	return Super::CanEditChange(InProperty);
}
#endif // WITH_EDITOR

