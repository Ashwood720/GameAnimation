// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OCLTLedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OCLT Ledge description")
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OCLT Ledge description")
	FVector LedgeNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OCLT Ledge description")
	FRotator Rotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OCLT_API UOCLTLedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	bool DetectLedge(OUT FLedgeDescription& LedgeDescription);

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
