// Copyright 2024 Richard Skala

#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
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
		// Move toward player
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

		// Face the player
		CurrentLocation = GetActorLocation();
		float FlipbookXScale = FlipbookComp->GetComponentScale().X;
		if ((PlayerLocation.X - CurrentLocation.X) >= 0.0f) // player is on the right side of the enemy (so enemy should face RIGHT)
		{
			if (FlipbookXScale < 0.0f)
			{
				FlipbookComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		else // player is on the left side of the enemy (so the enemy should face LEFT)
		{
			if (FlipbookXScale > 0.0f)
			{
				FlipbookComp->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void AEnemy::Die()
{
	UE_LOG(LogEnemy, Log, TEXT("AEnemy::OnDestroyTimerTimeout - %s"), *GetName());

	if (!bIsAlive)
	{
		return;
	}

	bIsAlive = false;
	bCanFollow = false;

	// Set the "dead" flipbook
	check(FlipbookComp != nullptr);
	FlipbookComp->SetFlipbook(DeadFlipbookAsset);

	// Change the sorting (so it is one layer behind what it was when alive)
	FlipbookComp->SetTranslucentSortPriority(-5);

	// Remove the dead enemy from the scene after X seconds
	const float DestroyTimeoutTimeSeconds = 5.0f;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::OnDestroyTimerTimeout, 1.0f, false, DestroyTimeoutTimeSeconds);
}

void AEnemy::SetPlayerTarget(ATopDownCharacter* InPlayer)
{
	Player = InPlayer;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*if (Player == nullptr)
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
	}*/
}

void AEnemy::OnDestroyTimerTimeout()
{
	UE_LOG(LogEnemy, Log, TEXT("AEnemy::OnDestroyTimerTimeout - %s"), *GetName());
	Destroy();
}

