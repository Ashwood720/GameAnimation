// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactive/OCLTInteractiveActor.h"
#include "OCLTLadder.generated.h"

class UAnimMontage;
class UBoxComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class OCLT_API AOCLTLadder : public AOCLTInteractiveActor
{
	GENERATED_BODY()
	
public:
	AOCLTLadder();

	virtual void BeginPlay() override;
	FVector GetAttachFromTopAnimMontageStartingLocation() const;
	UAnimMontage* GetAttachFromTopAnimMontage() const;
	bool GetIsOnTop() const;
	float GetLadderHeight() const;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		float LadderHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		float LadderWidth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		float StepsInterval = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInstancedStaticMeshComponent* StepsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* TopInteractionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		UAnimMontage* AttachFromTopAnimMontage;

	//Offset from ladder's top for starting anim montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OCLT Ladder parameters")
		FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

	UBoxComponent* GetLadderInteractionBox() const;

	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	bool bIsOnTop = false;
};
