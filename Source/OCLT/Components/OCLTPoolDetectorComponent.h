// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OCLTPoolDetectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OCLT_API UOCLTPoolDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool DetectPool();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f, ForceUnits = cm))
		float ForwardCheckDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f, ForceUnits = cm))
		float MinimumLedgeHeight = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OCLT Detection settings", meta = (UIMin = 0.0f, ClampMin = 0.0f, ForceUnits = cm))
		float MaximumLedgeHeight = 200.0f;

private:
	TWeakObjectPtr<ACharacter> CachedCharacterOwner;
};
