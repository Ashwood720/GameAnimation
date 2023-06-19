#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OCLTPlayerController.generated.h"

UCLASS()
class OCLT_API AOCLTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

private:
	void ChangeCrouchState();
	void ClimbLadderUp(float Value);
	void InteractWithLadder();
	void Jump();

	void LookUp(float Value);
	void Mantle();
	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartSprint();
	void StopSprint();
	void SwimForward(float Value);
	void SwimJump();
	void SwimRight(float Value);
	void SwimUp(float Value);

	void Turn(float Value);

	TSoftObjectPtr<class AOCLTBaseCharacter> CachedBaseCharacter;
};
