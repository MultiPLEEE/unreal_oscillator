// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    Camera -> SetupAttachment(GetRootComponent());
    Camera -> bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent -> BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent -> BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent -> BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
    
    PlayerInputComponent -> BindAxis("TurnCamera", this, &AMyCharacter::TurnCamera);
    PlayerInputComponent -> BindAxis("LookUp", this, &AMyCharacter::LookUp);
}

void AMyCharacter::MoveForward(float InputValue)
{
    FVector ForwardDirection = GetActorForwardVector();
    AddMovementInput(ForwardDirection, InputValue);
}

void AMyCharacter::MoveRight(float InputValue)
{
    FVector RightDirection = GetActorRightVector();
    AddMovementInput(RightDirection, InputValue);
}

void AMyCharacter::TurnCamera(float InputValue)
{
    AddControllerYawInput(InputValue);
}

void AMyCharacter::LookUp(float InputValue)
{
    AddControllerPitchInput(InputValue);
}
