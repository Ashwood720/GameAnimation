#include "Components/MovementComponents/OCLTCharacterMovementComponent.h"

#include <Characters/OCLTBaseCharacter.h>
#include <Actors/Interactive/Environment/OCLTLadder.h>

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"

//A
void UOCLTCharacterMovementComponent::AttachToLadder(const AOCLTLadder* Ladder)
{
	CurrentLadder = Ladder;
	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 180.0f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetActorToCurrentLadderProjection(GetActorLocation());

	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * CurrentLadder->GetActorForwardVector();

	if (CurrentLadder->GetIsOnTop())
	{
		NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	}
	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrientationRotation);
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}
//D
void UOCLTCharacterMovementComponent::DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod /* = EDetachFromLadderMethod::Fall*/)
{
	switch (DetachFromLadderMethod)
	{
	case EDetachFromLadderMethod::JumpOff:
	{
		FVector JumpDirection = CurrentLadder->GetActorForwardVector();
		SetMovementMode(MOVE_Falling);

		FVector JumpVelocity = JumpDirection * JumpOffFromLadderSpeed;

		ForceTargetRotation = JumpDirection.ToOrientationRotator();
		bForceRotation = true;

		GetOwner()->SetActorRotation(JumpDirection.ToOrientationRotator());
		Launch(JumpVelocity);
		break;
	}
	case EDetachFromLadderMethod::ReachingTheTop:
	{
		GetBaseCharacterOwner()->Mantle(true);
		break;
	}
	case EDetachFromLadderMethod::ReachingTheBottom:
	{
		SetMovementMode(MOVE_Walking);
		break;
	}
	case EDetachFromLadderMethod::Fall:
	default:
	{
		SetMovementMode(MOVE_Falling);
		break;
	}
	}
}
//E
void UOCLTCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}
void UOCLTCharacterMovementComponent::EndSwimJump()
{
}
//G
AOCLTBaseCharacter* UOCLTCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<AOCLTBaseCharacter*>(CharacterOwner);
}
const AOCLTLadder* UOCLTCharacterMovementComponent::GetCurrentLadder()
{
	return CurrentLadder;
}
float UOCLTCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
	}
	return Result;
}
float UOCLTCharacterMovementComponent::GetActorToCurrentLadderProjection(const FVector& Location) const
{
	checkf(IsValid(CurrentLadder), TEXT("UOCLTCharacterMovementComponent::GetCharacterToCurrentLadderProjection() cannot be invoke when curent ladder is null"));
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderToCharacterDistance = Location - CurrentLadder->GetActorLocation();
	return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}
float UOCLTCharacterMovementComponent::GetLadderRatioSpeed() const
{
	checkf(IsValid(CurrentLadder), TEXT("UOCLTCharacterMovementComponent::GetLadderRatioSpeed() cannot be invoke when curent ladder is null"));
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingOnLadderMaxSpeed;
}
//I
bool UOCLTCharacterMovementComponent::IsMantling() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}
bool UOCLTCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}
//S
void UOCLTCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingParameters)
{
	CurrentMantlingParameters = MantlingParameters;
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
}
void UOCLTCharacterMovementComponent::StartSwimJump()
{
}
void UOCLTCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}
void UOCLTCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}
//P
void UOCLTCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Mantling:
	{
		PhysMantling(DeltaTime, Iterations);
		break;
	}
	case (uint8)ECustomMovementMode::CMOVE_Ladder:
	{
		PhysLadder(DeltaTime, Iterations);
		break;
	}
	default:
		break;
	}
	Super::PhysCustom(DeltaTime, Iterations);

}
void UOCLTCharacterMovementComponent::PhysLadder(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBreakingDeceleration);
	FVector Delta = Velocity * DeltaTime;
	if (HasAnimRootMotion())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
		return;
	}
	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetActorToCurrentLadderProjection(NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheBottom);
		return;
	}
	else if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheTop);
		return;
	}
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
}
void UOCLTCharacterMovementComponent::PhysMantling(float DeltaTime, int32 Iterations)
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);

	float PositionAlpha = MantlingCurveValue.X;
	float XYCorrectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InitialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation, PositionAlpha);

	FVector Delta = NewLocation - GetActorLocation();

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}
void UOCLTCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (bForceRotation)
	{
		FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
		CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

		FRotator DeltaRot = GetDeltaRotation(DeltaTime);
		DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

		// Accumulate a desired new rotation.
		const float AngleTolerance = 1e-3f;

		if (!CurrentRotation.Equals(ForceTargetRotation, AngleTolerance))
		{
			FRotator DesiredRotation = ForceTargetRotation;
			// PITCH
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// YAW
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ROLL
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}

			// Set the new rotation.
			DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
			MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
		}
		else
		{
			ForceTargetRotation = FRotator::ZeroRotator;
			bForceRotation = false;
		}
		return;
	}
	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}
//O
void UOCLTCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}

	if (MovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
		{
			GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UOCLTCharacterMovementComponent::EndMantle, CurrentMantlingParameters.Duration, false);
		}

		default:
			break;
		}
	}
}