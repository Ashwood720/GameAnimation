// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/OCLTBaseCharacter.h"

#include "Components/MovementComponents/OCLTCharacterMovementComponent.h"
#include <Actors/Interactive/Environment/OCLTLadder.h>
#include <Components/OCLTLedgeDetectorComponent.h>
#include "Components/OCLTPoolDetectorComponent.h"

#include <Curves/CurveVector.h>
#include "Gameframework/CharacterMovementComponent.h"


AOCLTBaseCharacter::AOCLTBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UOCLTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	OCLTCharacterMovementComponent = StaticCast<UOCLTCharacterMovementComponent*>(GetCharacterMovement());
	OCLTLedgeDetectorComponent = CreateDefaultSubobject<UOCLTLedgeDetectorComponent>(TEXT("LedgeDetector"));
	OCLTPoolDetectorComponent = CreateDefaultSubobject<UOCLTPoolDetectorComponent>(TEXT("PoolDetector"));
}
//C
bool AOCLTBaseCharacter::CanSprint()
{
	return true;
}

void AOCLTBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching()) 
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}
bool AOCLTBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetOCLTCharacterMovementComponent()->IsMantling();
}
bool AOCLTBaseCharacter::CanMantle() const
{
	return !GetOCLTCharacterMovementComponent()->IsOnLadder() && !OCLTCharacterMovementComponent->IsMantling();
}
void AOCLTBaseCharacter::ClimbLadderUp(float Value)
{
	if (GetOCLTCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetOCLTCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}
//G
const AOCLTLadder* AOCLTBaseCharacter::GetAvailableLadder() const
{
	const AOCLTLadder* Result = nullptr;
	for (const AOCLTInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<AOCLTLadder>())
		{
			Result = StaticCast<const AOCLTLadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}
const FMantlingSettings& AOCLTBaseCharacter::GetMantlingSettings(float LedgeHeight)
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}
//I
void AOCLTBaseCharacter::InteractWithLadder()
{
	if (GetOCLTCharacterMovementComponent()->IsOnLadder())
	{
		GetOCLTCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	else
	{
		const AOCLTLadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());
			}
			GetOCLTCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}
	}
}

//M
void AOCLTBaseCharacter::Mantle(bool bForce /*= false*/)
{
	if(!(CanMantle() || bForce))
	{
		return;
	}
	FLedgeDescription LedgeDescription;
	if (OCLTLedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;
		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);

		MantlingParameters.Duration = MaxRange - MinRange;

		MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;

		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InitialAnimationLocation = MantlingParameters.TargetLocation - HighMantleSettings.AnimationCorrectionZ * FVector::UpVector + HighMantleSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		GetOCLTCharacterMovementComponent()->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

//R
void AOCLTBaseCharacter::RegisterInteractiveActor(AOCLTInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}
//S
void AOCLTBaseCharacter::StartSprint()
{
	if(bIsMoving && GetCharacterMovement()->IsMovingOnGround())
	{
		bIsSprintRequested = true;
		if (bIsCrouched)
		{
			UnCrouch();
		}
	}
}
void AOCLTBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}
void AOCLTBaseCharacter::SwimJump()
{
	if(GetCharacterMovement()->IsMovingOnGround() && OCLTPoolDetectorComponent->DetectPool() && bIsMoving)
	{
		PlayAnimMontage(SwimJumpMontage);
	}
}

//T
void AOCLTBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState();
}
void AOCLTBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !OCLTCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		OCLTCharacterMovementComponent->StartSprint();
	}
	if (!bIsSprintRequested && OCLTCharacterMovementComponent->IsSprinting())
	{
		OCLTCharacterMovementComponent->StopSprint();
	}
}
//U
void AOCLTBaseCharacter::UnregisterInteractiveActor(AOCLTInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

//void AOCLTBaseCharacter::OnSprintStart_Implementation(){}
//void AOCLTBaseCharacter::OnSprintStop_Implementation(){}