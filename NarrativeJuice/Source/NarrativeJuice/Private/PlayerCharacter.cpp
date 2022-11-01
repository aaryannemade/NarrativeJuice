// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	DodgeRoll();
	
	SetHadMovementInputLastFrame(MovementInput);

	

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Desired Animation Rotation: %f"), AnimationTargetRotation-AnimationTargetRotationLastFrame));

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("MousePitch", this, &APlayerCharacter::MousePitch);
	PlayerInputComponent->BindAxis("MouseYaw", this, &APlayerCharacter::MouseYaw);
	PlayerInputComponent->BindAxis("GamepadPitch", this, &APlayerCharacter::GamepadPitch);
	PlayerInputComponent->BindAxis("GamepadYaw", this, &APlayerCharacter::GamepadYaw);

	PlayerInputComponent->BindAction("DodgeRoll", EInputEvent::IE_Pressed, this, &APlayerCharacter::Dodge);

}

void APlayerCharacter::Dodge()
{
	IsDodging = true;
	//SetActorRotation(DesiredMovementRotation);
	SetMovementRotationMode(E_MovementRotationMode::RootMotionRotation);
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

void APlayerCharacter::MousePitch(float value)
{
	AddControllerPitchInput(value * MouseSensitivity);
}

void APlayerCharacter::MouseYaw(float value)
{
	AddControllerYawInput(value * MouseSensitivity);
}

void APlayerCharacter::GamepadPitch(float value)
{
	AddControllerPitchInput(value * DeltaX * ControllerSensitivity);
}

void APlayerCharacter::GamepadYaw(float value)
{
	AddControllerYawInput(value * DeltaX * ControllerSensitivity);
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

		SetLeanDirection();

		if(MovementSpeed == E_MovementSpeed::Jog)
		{
			AutoRun();
		}
	}
	else
	{
		MovementInput = false;

		SetMovementRotationMode(E_MovementRotationMode::RootMotionRotation);

		AnimationTargetRotationLastFrame = 0.f;

		AddCharacterStoppingMovement();

		if(JogTimer >0.f)
		{
			JogTimer = 0.f;
		}

		
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
		
		if(MovementSpeed == E_MovementSpeed::Run)
		{
			MovementSpeed = E_MovementSpeed::Jog;
			UpdateCharacterMaxWalkSpeed();
		}
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
	FRotator TargetRotation = FRotator::ZeroRotator;
	float TargetAnimationRotation = 0.f;
	switch (MovementRotationMode)
	{
	case E_MovementRotationMode::MovementInputRotation:
		//FRotator TargetRotation = FRotator::ZeroRotator;
		TargetRotation = FMath::Lerp(GetActorRotation(), DesiredMovementRotation, MovementRotationSpeed * DeltaX);
		SetActorRotation(TargetRotation, ETeleportType::None);
		GEngine->AddOnScreenDebugMessage(-2, DeltaX, FColor::Red, TEXT("Using Movement Input Rotation"));
		break;
		
	case E_MovementRotationMode::RootMotionRotation:
		if(MovementInput && !IsDodging)
		{
			TargetAnimationRotation = AnimationTargetRotation - AnimationTargetRotationLastFrame;
			AddActorWorldRotation(FRotator(0, TargetAnimationRotation, 0), false, nullptr, ETeleportType::None);
			AnimationTargetRotationLastFrame = AnimationTargetRotation;
		}
		
		GEngine->AddOnScreenDebugMessage(-2, DeltaX, FColor::Red, TEXT("Using Animation Curve Rotation"));
		break;
		
	default:
		break;
	}
}

void APlayerCharacter::AddCharacterStoppingMovement()
{
	AddMovementInput(GetActorForwardVector(), 1.f);
}

void APlayerCharacter::UpdateCharacterMaxWalkSpeed()
{
	switch(MovementSpeed)
	{
	case E_MovementSpeed::Walk:
		GetCharacterMovement()->MaxWalkSpeed = MovementSettings.WalkSpeed;
		break;
	case E_MovementSpeed::Jog:
		GetCharacterMovement()->MaxWalkSpeed = MovementSettings.JogSpeed;
		break;
	case E_MovementSpeed::Run:
		GetCharacterMovement()->MaxWalkSpeed = MovementSettings.RunSpeed;
		break;
	default:
		break;
	}
}

void APlayerCharacter::UpdateCharacterMovementSpeed(float MoveSpeed)
{
	switch (MovementRotationMode)
	{
	case E_MovementRotationMode::MovementInputRotation:
		UpdateCharacterMaxWalkSpeed();
		break;
	case E_MovementRotationMode::RootMotionRotation:
		if(MoveSpeed < 25.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		}
		break;
	default:
		break;
	}
}


void APlayerCharacter::UpdateAnimationTargetRotation(float rotation)
{
	AnimationTargetRotation = rotation;
}

void APlayerCharacter::SetLeanDirection()
{
	float DeltaYaw = ((DesiredMovementRotation - GetActorRotation()).GetNormalized()).Yaw;
	float LeanRaw = FMath::GetMappedRangeValueClamped(FVector2d(-LeanThreshold, LeanThreshold), FVector2d(-1, 1), DeltaYaw);

	LeanDirection = FMath::Lerp(LeanDirection, LeanRaw, LeanSpeed * DeltaX);
}

void APlayerCharacter::AutoRun()
{
	if(JogTimer > MovementSettings.AutoRunTime)
	{
		MovementSpeed = E_MovementSpeed::Run;
		UpdateCharacterMaxWalkSpeed();
		JogTimer = 0.f;
	}
	else
	{
		JogTimer += DeltaX;
	}
}

void APlayerCharacter::DodgeRoll()
{
	if(IsDodging)
	{
		DodgeTimer += DeltaX;
		if(DodgeTimer < 0.44f)
		{
			GetCharacterMovement()->Velocity = GetActorForwardVector() * MovementSettings.DodgeForce;
		}
		else if(DodgeTimer < 0.8f)
		{
			SetMovementRotationMode(E_MovementRotationMode::RootMotionRotation);
		}
		else
		{
			SetMovementRotationMode(E_MovementRotationMode::MovementInputRotation);
			IsDodging = false;
			DodgeTimer = 0.f;
		}
	}
}





