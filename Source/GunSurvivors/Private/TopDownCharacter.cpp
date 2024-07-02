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
}

