// Copyright 2024 Richard Skala


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"

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
	MovementDirection = FVector2D(1.0f, 0.0f);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update bullet movement
	FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);

	SetActorLocation(NewLocation);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}
