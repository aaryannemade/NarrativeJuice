// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovementComponentAsync.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum E_MovementRotationMode
{
	RootMotionRotation	UMETA(DisplayName = "Animation Root Motion"),
	MovementInputRotation	UMETA(DisplayName = "Movement Input"),
};

UCLASS()
class NARRATIVEJUICE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	float DeltaX = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveX = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveY = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ControllerSensitivity = 45.f;

	void MoveForward(float value);

	void MoveRight(float value);

	void MousePitch(float value);
	void MouseYaw(float value);
	void GamepadPitch(float value);
	void GamepadYaw(float value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool MovementInput = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool HadMovementInputLastFrame = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator DesiredMovementRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MovementRotationSpeed = 6.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementStartDirection = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<E_MovementRotationMode> MovementRotationMode = E_MovementRotationMode::RootMotionRotation;

	FRotator GetControlRotationYaw();

	void SetDesiredMovementRotation(float InputX, float InputY, FRotator ControlRotation);

	void SetMovementInput(float InputX, float InputY);

	void SetHadMovementInputLastFrame(bool movementInput);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool StartedMovement();

	void SetMovementStartDirection();

	float CalculateMovementStartDirection(float InputX, float InputY, FRotator WorldRotation, FRotator ControlRotation);

	void SetMovementRotationMode(E_MovementRotationMode RotationMode);

	void SetCharacterRotation();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};




