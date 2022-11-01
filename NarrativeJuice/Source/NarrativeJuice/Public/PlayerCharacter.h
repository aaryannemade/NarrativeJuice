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
	MovementInputRotation	UMETA(DisplayName = "Movement Input")
};

UENUM(BlueprintType)
enum E_MovementSpeed
{
	Walk	UMETA(DisplayerName = "Walk"),
	Jog		UMETA(DisplayName = "Jog"),
	Run		UMETA(DisplayName = "Run")
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 165.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JogSpeed = 345.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed = 695.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoRunTime = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DodgeForce = 1560.f;
};

UCLASS()
class NARRATIVEJUICE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovementSettings MovementSettings = FMovementSettings();

	float DeltaX = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveX = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveY = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MouseSensitivity = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ControllerSensitivity = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimationTargetRotation = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AnimationTargetRotationLastFrame = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDodging = false;

	float DodgeTimer = 0.f;

	void MoveForward(float value);

	void MoveRight(float value);

	void MousePitch(float value);
	void MouseYaw(float value);
	void GamepadPitch(float value);
	void GamepadYaw(float value);

	void Dodge();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<E_MovementSpeed> MovementSpeed = E_MovementSpeed::Jog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float JogTimer = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LeanDirection = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeanThreshold = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeanSpeed = 3.f;
	
	FRotator GetControlRotationYaw();

	void SetDesiredMovementRotation(float InputX, float InputY, FRotator ControlRotation);

	void SetMovementInput(float InputX, float InputY);

	void SetHadMovementInputLastFrame(bool movementInput);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool StartedMovement();

	void SetMovementStartDirection();

	float CalculateMovementStartDirection(float InputX, float InputY, FRotator WorldRotation, FRotator ControlRotation);

	UFUNCTION(BlueprintCallable)
	void SetMovementRotationMode(E_MovementRotationMode RotationMode);

	void SetCharacterRotation();

	void AddCharacterStoppingMovement();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterMaxWalkSpeed();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterMovementSpeed(float MoveSpeed);

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationTargetRotation(float rotation);

	void SetLeanDirection();

	void AutoRun();

	void DodgeRoll();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};




