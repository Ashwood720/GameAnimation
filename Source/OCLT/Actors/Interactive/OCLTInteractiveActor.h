// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OCLTInteractiveActor.generated.h"

UCLASS(Abstract, NotBlueprintable)
class OCLT_API AOCLTInteractiveActor : public AActor
{
	GENERATED_BODY()
	


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "OCLT Interaction")
		class UPrimitiveComponent* InteractionVolume;

	UFUNCTION()
	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	UFUNCTION()
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
