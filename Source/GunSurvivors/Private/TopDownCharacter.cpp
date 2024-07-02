// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "PaperFlipbookComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTopDownCharacter, Log, All)

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::ATopDownCharacter - %s"), *GetName());

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	CharacterFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbookComponent"));
	CharacterFlipbookComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::BeginPlay - %s"), *GetName());

	Super::BeginPlay();

	// Register Inputs from the EnhancedInputLocalPlayerSubsystem (which is accessed from the LocalPlayer)
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//InputSubsystem->AddMappingContext(InputMappingContext, 0, FModifyContextOptions());
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::MoveTriggered - %s - MoveActionValue: %s"), *GetName(), *MoveActionValue.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, MoveActionValue.ToString());
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::MoveCompleted - %s - InputActionValue: %s"), *GetName(), *InputActionValue.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, InputActionValue.ToString());
}

void ATopDownCharacter::Shoot(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTopDownCharacter, Log, TEXT("ATopDownCharacter::Shoot - %s - InputActionValue: %s"), *GetName(), *InputActionValue.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, InputActionValue.ToString());
}

