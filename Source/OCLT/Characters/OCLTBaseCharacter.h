#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Curves/CurveVector.h>
#include "OCLTBaseCharacter.generated.h"

class UOCLTCharacterMovementComponent;
class AOCLTInteractiveActor;

typedef TArray<AOCLTInteractiveActor*, TInlineAllocator<10>> TInteractiveActorsArray;

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

UCLASS(Abstract, NotBlueprintable)
class OCLT_API AOCLTBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOCLTBaseCharacter(const FObjectInitializer& ObjectInitializer);
	//C
	virtual void ChangeCrouchState();
	virtual bool CanJumpInternal_Implementation() const override;
	bool CanMantle() const;
	//G
	const class AOCLTLadder* GetAvailableLadder() const;
	FORCEINLINE UOCLTCharacterMovementComponent* GetOCLTCharacterMovementComponent() const {return OCLTCharacterMovementComponent;}

	void ClimbLadderUp(float Value);

	void InteractWithLadder();

	virtual void LookUp(float Value) {};
	//M
	virtual void Mantle(bool bForce = false);
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	//R
	void RegisterInteractiveActor(AOCLTInteractiveActor* InteractiveActor);
	//Q
	void UnregisterInteractiveActor(AOCLTInteractiveActor* InteractiveActor);

	


	virtual void StartSprint();
	virtual void StopSprint();
	virtual void SwimForward(float Value) {};
	virtual void SwimJump();
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void Tick(float DeltaTime) override;
	virtual void Turn(float Value) {};

protected:
	/*UPROPERTY(BlueprintNativeEvent, Category = "OCLT Character | Movement")
		void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UPROPERTY(BlueprintNativeEvent, Category = "OCLT Character | Movement")
		void OnSprintEnd();
	virtual void OnSprintStop_Implementation();*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OCLT Character | Movement")
		class UOCLTLedgeDetectorComponent* OCLTLedgeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OCLT Character | Movement")
		class UOCLTPoolDetectorComponent* OCLTPoolDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Character | Movement")
		FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Character | Movement")
		FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Character | Movement", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float LowMantleMaxHeight = 125.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UAnimMontage* SwimJumpMontage;


	virtual bool CanSprint();

	UOCLTCharacterMovementComponent* OCLTCharacterMovementComponent;

	bool bIsMoving = false;
private:

	bool bIsSprintRequested = false;
	void TryChangeSprintState();

	const FMantlingSettings& GetMantlingSettings(float LedgeHeight);

	TInteractiveActorsArray AvailableInteractiveActors;
};
