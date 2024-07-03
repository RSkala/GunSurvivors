// Copyright 2024 Richard Skala

#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

#include "TopDownCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemy, Log, All)

// Sets default values
AEnemy::AEnemy()
{
	UE_LOG(LogEnemy, Log, TEXT("AEnemy::AEnemy - %s"), *GetName());

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookComp->SetupAttachment(RootComponent);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Enemy movement
	if (bIsAlive && bCanFollow && Player.IsValid())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		FVector DirEnemyToPlayer = PlayerLocation - CurrentLocation;
		float DistanceToPlayer = DirEnemyToPlayer.Length();

		if (DistanceToPlayer >= StopDistance)
		{
			DirEnemyToPlayer.Normalize();
			FVector NewLocation = CurrentLocation + (DirEnemyToPlayer * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (Player == nullptr)
	{
		UE_LOG(LogEnemy, Log, TEXT("AEnemy::BeginPlay - %s - Player is nullptr"), *GetName());
	}

	if (!Player.IsValid())
	{
		UE_LOG(LogEnemy, Log, TEXT("AEnemy::BeginPlay - %s - Player is NOT valid... setting it"), *GetName());

		AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (PlayerActor != nullptr)
		{
			UE_LOG(LogEnemy, Log, TEXT("AEnemy::BeginPlay - %s - Set the player!"), *GetName());
			Player = Cast<ATopDownCharacter>(PlayerActor);
			bCanFollow = true;
		}
	}
}

