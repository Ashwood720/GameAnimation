// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OCLTBaseCharacterAnimInstance.generated.h"

UCLASS()
class OCLT_API UOCLTBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Character | Animation")
		float LadderSpeedRatio = 0.0f;

private:
	TWeakObjectPtr<class AOCLTBaseCharacter> CachedBaseCharacter;
};
