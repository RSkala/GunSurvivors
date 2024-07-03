// Copyright 2024 Richard Skala

#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBullet, Log, All)

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	BulletSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSpriteComp"));
	BulletSpriteComp->SetupAttachment(RootComponent);

	// Initialize the movement direction to move to the right
	//MovementDirection = FVector2D(1.0f, 0.0f);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLaunched)
	{
		// Update bullet movement
		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);

		SetActorLocation(NewLocation);
	}
}

void ABullet::Launch(FVector2D InMovementDirection, float InMovementSpeed)
{
	UE_LOG(LogBullet, Log, TEXT("ABullet::Launch - %s"), *GetName());

	if (bIsLaunched) return;
	bIsLaunched = true;

	MovementDirection = InMovementDirection;
	MovementSpeed = InMovementSpeed;

	// Start delete timer
	const float DeleteTime = 10.0f;
	GetWorldTimerManager().SetTimer(DeleteTimer, this, &ThisClass::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	UE_LOG(LogBullet, Log, TEXT("ABullet::BeginPlay - %s"), *GetName());
	Super::BeginPlay();

	// test:
	//Launch(FVector2D(0.0f, 1.0f), 25.0f);
}

void ABullet::OnDeleteTimerTimeout()
{
	UE_LOG(LogBullet, Log, TEXT("ABullet::OnDeleteTimerTimeout - %s"), *GetName());
	Destroy();
}
