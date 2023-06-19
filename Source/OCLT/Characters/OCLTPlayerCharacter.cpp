// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/OCLTPlayerCharacter.h"

#include "Components/MovementComponents/OCLTCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AOCLTPlayerCharacter::AOCLTPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseControllerRotationPitch = 0; // |
	bUseControllerRotationYaw = 0;   // } управление персонажем контроллером отключено, т.е. персонаж не вращаетс€ мышью
	bUseControllerRotationRoll = 0;  // |

	//≈сли SpringArmComponent будет корневым компонентом (RootComponent), то он ќЅя«ј“≈Ћ№Ќќ должен быть первым!!!

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm")); //SpringArmComponent добавл€етс€ в AOCLTPlayerCharacter
	SpringArmComponent->SetupAttachment(RootComponent); //SpringArmComponent назначаетс€ базовым компонентом (RootComponent)
	SpringArmComponent->bUsePawnControlRotation = 1; //SpringArmComponent ”ѕ–ј¬Ћя≈“—я ћџЎ№ё

	//¬торой компонент

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera component")); //CameraComponent добавл€етс€ в AOCLTPlayerCharacter
	CameraComponent->SetupAttachment(SpringArmComponent); //CameraComponent вложен в SpringArmComponent
	CameraComponent->bUsePawnControlRotation = 0; //CameraComponent отключен от управлени€ мышью

	GetCharacterMovement()->bOrientRotationToMovement = 1; //AOCLTPlayerCharacter повернетьс€ в завимимости WASD
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // скорость поворота не превышает 540? градусов в секунду
}

bool AOCLTPlayerCharacter::CanJumpInternal_Implementation() const
{
	return bIsCrouched || Super::CanJumpInternal_Implementation();
}

void AOCLTPlayerCharacter::MoveForward(float Value)
{
	if(GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
		bIsMoving = true;
	}
	if (FMath::IsNearlyZero(Value, 1e-6f))
	{
		bIsMoving = false;
	}
}
void AOCLTPlayerCharacter::MoveRight(float Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
		//AddMovementInput(GetActorRightVector(), Value);
	}
}

void AOCLTPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AOCLTPlayerCharacter::SwimForward(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator PitchYawRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = PitchYawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AOCLTPlayerCharacter::SwimRight(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
		//AddMovementInput(GetActorRightVector(), Value);
	}
}
void AOCLTPlayerCharacter::SwimUp(float Value)
{
	if (GetCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6f))
	{
		AddMovementInput(FVector::UpVector, Value);
	}
}

void AOCLTPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}
void AOCLTPlayerCharacter::OnJumped_Implementation()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
}
void AOCLTPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void AOCLTPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
