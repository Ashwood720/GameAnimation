// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/OCLTLedgeDetectorComponent.h"

#include <OCLTTypes.h>
#include <Utils/OCLTTraceUtils.h>
#include "Subsystems/OCLTDebugSubsystem.h"
#include <Characters/OCLTBaseCharacter.h>
#include <OCLTGameInstance.h>

#include <Components/CapsuleComponent.h>
#include <GameFramework/Character.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

void UOCLTLedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("UOCLTLedgeDetectorComponent::BeginPlay() only a character can use UOCLTLedgeDetectorComponent"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

bool UOCLTLedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
#if ENABLE_DRAW_DEBUG
	UOCLTDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UOCLTDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else 
	bool bIsDebugEnabled = false;
#endif
	float DrawTime = 2.0f;
	float BottomZOffset = 2.0f;
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;

	//1. Forward check 
	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5f;

	FHitResult ForwardCheckHitResult;
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	if (!OCLTTraceUtils::SweepCapsuleSingleByChannel(GetWorld(), ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FQuat::Identity, UE_ECC_CLIMBING, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	//2. Downward check
	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();

	float DownwardCheckDepthOffset = 10.0f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	if (!OCLTTraceUtils::SweepSphereSingleByChannel(GetWorld(), DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, DownwardSphereCheckRadius, UE_ECC_CLIMBING, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	
	//3. Overlap check 
	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);

	float OverlapCapsuleFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsuleFloorOffset) * FVector::UpVector;
	if (OCLTTraceUtils::OverlapCapsuleBlockingByProfile(GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, CollisionProfilePawn, QueryParams, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	LedgeDescription.Location = OverlapLocation;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();
	LedgeDescription.LedgeNormal = ForwardCheckHitResult.ImpactNormal;

	return true;
}