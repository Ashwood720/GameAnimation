#pragma once

#include "CoreMinimal.h"
#include "OCLTBaseCharacter.h"
#include "OCLTPlayerCharacter.generated.h"

UCLASS(Blueprintable)
class OCLT_API AOCLTPlayerCharacter : public AOCLTBaseCharacter
{
	GENERATED_BODY()
	
public:
	AOCLTPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual bool CanJumpInternal_Implementation() const;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	virtual void LookUp(float Value) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnJumped_Implementation();
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void SwimForward(float Value) override;
	virtual void SwimRight(float Value) override;
	virtual void SwimUp(float Value) override;

	virtual void Turn(float Value) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "OCLT Character | Camera")
		class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "OCLT Character | Camera")
		class USpringArmComponent* SpringArmComponent;

};
