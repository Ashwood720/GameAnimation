#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/OCLTLedgeDetectorComponent.h>
#include "OCLTCharacterMovementComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 2.0f;
	float StartTime = 0.0f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDetachFromLadderMethod : uint8
{
	Fall = 0,
	ReachingTheTop,
	ReachingTheBottom,
	JumpOff
};

UCLASS()
class OCLT_API UOCLTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	//A
	void AttachToLadder(const class AOCLTLadder* Ladder);
	//D
	void DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod = EDetachFromLadderMethod::Fall);
	//E
	void EndMantle();
	void EndSwimJump();
	//G
	class AOCLTBaseCharacter* GetBaseCharacterOwner() const;
	const class AOCLTLadder* GetCurrentLadder();
	virtual float GetMaxSpeed() const override;
	float GetActorToCurrentLadderProjection(const FVector& Location) const;
	float GetLadderRatioSpeed() const;
	//I
	FORCEINLINE bool IsSprinting() {return bIsSprinting;}
	//P
	virtual void PhysicsRotation(float DeltaTime) override;
	//S
	void StartMantle(const FMantlingMovementParameters& MantlingMovementParameters);
	void StartSwimJump();

	void StartSprint();
	void StopSprint();
	//I
	bool IsMantling() const;
	bool IsOnLadder() const;

	protected:

		virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
		void PhysMantling(float DeltaTime, int32 Iterations);
		void PhysLadder(float DeltaTime, int32 Iterations);

		virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f, ForceUnits = cm))
			float SprintSpeed = 1200.0f;

		UPROPERTY(Category = "OCLT Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float SwimmingCapsuleRadius = 60.0f;

		UPROPERTY(Category = "OCLT Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float SwimmingCapsuleHalfHeight = 60.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float ClimbingOnLadderMaxSpeed = 200.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float LadderToCharacterOffset = 60.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float ClimbingOnLadderBreakingDeceleration = 2048.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float MaxLadderTopOffset = 90.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float MinLadderBottomOffset = 90.0f;

		UPROPERTY(Category = "OCLT Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
			float JumpOffFromLadderSpeed = 500.0f;

private:
	bool bIsSprinting = false;

	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;

	const AOCLTLadder* CurrentLadder = nullptr;

	FRotator ForceTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;
};
