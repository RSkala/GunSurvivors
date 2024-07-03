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
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}



