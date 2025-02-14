// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
//#include "Engine/TimerHandle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"

#include "Bullet.h"
#include "Enemy.h"

DEFINE_LOG_CATEGORY_STATIC(LogTopDownCharacter, Log, All)

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::ATopDownCharacter - %s"), *GetName());

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create player character components
	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbookComponent"));
	CharacterFlipbookComponent->SetupAttachment(RootComponent);

	// Create gun components
	//DummyGunComp = CreateDefaultSubobject<USceneComponent>(TEXT("DummyGunComp"));
	//DummyGunComp->SetupAttachment(RootComponent);

	GunParentComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunParentComp"));
	GunParentComp->SetupAttachment(RootComponent);

	GunSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSpriteComp"));
	GunSpriteComp->SetupAttachment(GunParentComp);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSpriteComp);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::BeginPlay - %s"), *GetName());

	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Ensure the mouse cursor is always visible
		PlayerController->SetShowMouseCursor(true);

		// Register Inputs from the EnhancedInputLocalPlayerSubsystem (which is accessed from the LocalPlayer)
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//InputSubsystem->AddMappingContext(InputMappingContext, 0, FModifyContextOptions());
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Delegates
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapBegin);
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if player can move
	if (bCanMove)
	{
		// Check if player has any input
		if (MovementDirection.Length() > 0.0f)
		{
			if (MovementDirection.Length() > 1.0f)
			{
				MovementDirection.Normalize(); // Note: We could also go MovementDirection.GetSafeNormal() and use that
			}

			// Get the distance to move this frame using the movement direction
			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

			// Get the current position of the player at this frame
			FVector CurrentLocation = GetActorLocation();

			// The player moves in the XZ plane
			//FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);

			// Move the player in the X (left/right) direction
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);

			// Check if the new location is outside of the horizontal map bounds
			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				// This new location is OUTSIDE the horizontal map bounds. "Take back" (remove) the new X position from the new location.
				NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
			}

			// Move the player in the Z (up/down) direction
			NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				// This new location is OUTSIDE the vertical map bounds. "Take back" (remove) the new Z position from the new location.
				NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
			}

			// Set the player's new location
			SetActorLocation(NewLocation);
		}
	}

	// Rotate the gun
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the player's position in the XZ plane
		FVector CurrentLocation = GetActorLocation();
		FVector StartPos = FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z);

		// Get the mouse position in world coordinates in the XZ plane
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
		FVector TargetPos = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);

		// Create a rotation representing the vector from the player position to the mouse position
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(StartPos, TargetPos);

		// Set the GunParentComponent's rotation
		GunParentComp->SetRelativeRotation(GunParentRotator);
	}
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::SetupPlayerInputComponent - %s"), *GetName());

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Canceled, this, &ThisClass::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Started, this, &ThisClass::Shoot);
		EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ThisClass::Shoot);
	}
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& InputActionValue)
{
	FVector2D MoveActionValue = InputActionValue.Get<FVector2d>();
	//UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::MoveTriggered - %s - MoveActionValue: %s"), *GetName(), *MoveActionValue.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, MoveActionValue.ToString());

	if (bCanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbookComponent->SetFlipbook(RunFlipbook);

		FVector FlipbookScale = CharacterFlipbookComponent->GetComponentScale();
		if (MovementDirection.X < 0.0f)
		{
			// Moving Left
			if (FlipbookScale.X > 0.0f)
			{
				// Flip the flipbook scale to face left (if it hasn't been flipped already)
				CharacterFlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
		else if (MovementDirection.X > 0.0f)
		{
			// Moving Right
			if (FlipbookScale.X < 0.0f)
			{
				// Flip the flipbook scale to face right (if it hasn't been flipped already)
				CharacterFlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::MoveCompleted - %s - InputActionValue: %s"), *GetName(), *InputActionValue.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, InputActionValue.ToString());
	//MovementDirection = FVector2D(0.0f, 0.0f);
	MovementDirection = FVector2D::ZeroVector;

	if (bIsAlive)
	{
		CharacterFlipbookComponent->SetFlipbook(IdleFlipbook);
	}
}

void ATopDownCharacter::Shoot(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::Shoot - %s - InputActionValue: %s"), *GetName(), *InputActionValue.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, InputActionValue.ToString());

	if (bCanShoot)
	{
		bCanShoot = false;

		// Spawn the bullet
		UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::Shoot - %s"), *GetName());
		ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPosition->GetComponentLocation(), FRotator::ZeroRotator);
		check(SpawnedBullet != nullptr);

		// Get the world mouse position
		FVector MouseWorldLocation, MouseWorldDirection;
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		check(PlayerController != nullptr);
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Calculate bullet direction
		FVector CurrentLocation = GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection.Normalize();

		// Launch the bullet
		const float BulletSpeed = 300.0f;
		SpawnedBullet->Launch(BulletDirection, BulletSpeed);

		// Play the shoot sound
		UGameplayStatics::PlaySound2D(GetWorld(), BulletShootSound);

		// Start the timer that allows the player to shoot again
		GetWorldTimerManager().SetTimer(ShootCooldownTimerHandle, this, &ThisClass::OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownDurationSeconds);
	}
}

void ATopDownCharacter::OnShootCooldownTimerTimeout()
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::OnShootCooldownTimerTimeout - %s"), *GetName());
	if (bIsAlive)
	{
		bCanShoot = true;
	}
}

bool ATopDownCharacter::IsInMapBoundsHorizontal(float XPos) const
{
	bool bResult = true;
	//UE_LOG(LogTopDownCharacter, Log, TEXT("----------------------------------------"));
	//UE_LOG(LogTopDownCharacter, Log, TEXT("IsInMapBoundsHorizontal"));
	//UE_LOG(LogTopDownCharacter, Log, TEXT(" - XPos:               %f"), XPos);
	//UE_LOG(LogTopDownCharacter, Log, TEXT(" - HorizontalLimits.X: %f"), HorizontalLimits.X);
	//UE_LOG(LogTopDownCharacter, Log, TEXT(" - HorizontalLimits.Y: %f"), HorizontalLimits.Y);
	bResult = (XPos >= HorizontalLimits.X) && (XPos <= HorizontalLimits.Y);
	//UE_LOG(LogTopDownCharacter, Log, TEXT(" - bResult:            %d"), bResult);
	return bResult;
}

bool ATopDownCharacter::IsInMapBoundsVertical(float ZPos) const
{
	bool bResult = true;
	/*UE_LOG(LogTopDownCharacter, Log, TEXT("----------------------------------------"));
	UE_LOG(LogTopDownCharacter, Log, TEXT("IsInMapBoundsVertical"));
	UE_LOG(LogTopDownCharacter, Log, TEXT(" - ZPos:             %f"), ZPos);
	UE_LOG(LogTopDownCharacter, Log, TEXT(" - VerticalLimits.X: %f"), VerticalLimits.X);
	UE_LOG(LogTopDownCharacter, Log, TEXT(" - VerticalLimits.Y: %f"), VerticalLimits.Y);*/
	bResult = (ZPos >= VerticalLimits.X) && (ZPos <= VerticalLimits.Y);
	//UE_LOG(LogTopDownCharacter, Log, TEXT(" - bResult:            %d"), bResult);
	return bResult;
}

void ATopDownCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	FString OtherActorName = OtherActor != nullptr ? OtherActor->GetName() : TEXT("(invalid)");
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::OverlapBegin - %s, OtherActor: %s"), *GetName(), *OtherActorName);

	if (AEnemy* OverlappedEnemy = Cast<AEnemy>(OtherActor))
	{
		if (OverlappedEnemy->IsAlive())
		{
			if (bIsAlive)
			{
				// Player is alive and has collided with an Enemy. Mark player as dead, and disable moving and shooting.
				bIsAlive = false;
				bCanMove = false;
				bCanShoot = false;

				UGameplayStatics::PlaySound2D(GetWorld(), DieSound);

				PlayerDiedDelegate.Broadcast();
			}
		}
	}
}
