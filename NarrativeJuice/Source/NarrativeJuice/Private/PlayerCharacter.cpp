// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaX = DeltaTime;

	SetMovementInput(MoveX, MoveY);

	SetDesiredMovementRotation(MoveX, MoveY, GetControlRotationYaw());

	SetCharacterRotation();

	SetMovementStartDirection();
	
	SetHadMovementInputLastFrame(MovementInput);

	

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Desired Movement Rotation: ") + DesiredMovementRotation.ToString()));

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

}

void APlayerCharacter::MoveForward(float value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Move Forward is being called"));
	
	//Set up input move x value
	MoveX = value;

	//Add forward/back movement to character
	const FVector Direction = FRotationMatrix(GetControlRotationYaw()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, MoveX);
}

void APlayerCharacter::MoveRight(float value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Move Right is being called"));
	
	//set up input move y value
	MoveY = value;

	//Add right/left movement to character
	const FVector Direction = FRotationMatrix(GetControlRotationYaw()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, MoveY);
}

FRotator APlayerCharacter::GetControlRotationYaw()
{
	const FRotator ControlRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

	return ControlRotation;
}

void APlayerCharacter::SetDesiredMovementRotation(float InputX, float InputY, FRotator ControlRotation)
{
	FVector MovementInputVector = FVector(InputX, -InputY, 0);

	FRotator MovementInputRotator = MovementInputVector.Rotation();

	DesiredMovementRotation = ControlRotation - MovementInputRotator;
	
}

void APlayerCharacter::SetMovementInput(float InputX, float InputY)
{
	if(FMath::Abs(InputX) != 0.f || FMath::Abs(InputY) != 0.f)
	{
		MovementInput = true;
	}
	else
	{
		MovementInput = false;

		SetMovementRotationMode(E_MovementRotationMode::RootMotionRotation);
	}
}

void APlayerCharacter::SetHadMovementInputLastFrame(bool movementInput)
{
	HadMovementInputLastFrame = movementInput;
}

bool APlayerCharacter::StartedMovement()
{
	if(MovementInput && !HadMovementInputLastFrame)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartedMoving"));

		return true;
	}
	else
	{
		return false;
	}
}

void APlayerCharacter::SetMovementStartDirection()
{
	if(StartedMovement())
	{
		MovementStartDirection = CalculateMovementStartDirection(MoveX, MoveY, GetCapsuleComponent()->GetComponentRotation(), GetControlRotationYaw());
	}
}

float APlayerCharacter::CalculateMovementStartDirection(float InputX, float InputY, FRotator WorldRotation,
	FRotator ControlRotation)
{
	//Calculate absolute input movement rotation
	FVector MovementInputVector = FVector(InputX, -InputY, 0.f);
	FRotator MovementInputRotation = MovementInputVector.Rotation();

	//Calculate World Offset Rotation
	FRotator CurrentRotation = ControlRotation - WorldRotation;

	//Calculate Start Rotation
	FRotator StartDirection = CurrentRotation - MovementInputRotation;
	StartDirection = StartDirection.GetNormalized();

	return StartDirection.Yaw;
}

void APlayerCharacter::SetMovementRotationMode(E_MovementRotationMode RotationMode)
{
	MovementRotationMode = RotationMode;
}

void APlayerCharacter::SetCharacterRotation()
{
	if(MovementInput && MovementRotationMode == E_MovementRotationMode::MovementInputRotation)
	{
		FRotator TargetRotation = FMath::Lerp(GetActorRotation(), DesiredMovementRotation, MovementRotationSpeed * DeltaX);
		SetActorRotation(TargetRotation, ETeleportType::None);
	}
}



