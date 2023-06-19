// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OCLTPoolDetectorComponent.h"

#include <OCLTTypes.h>
#include <Utils/OCLTTraceUtils.h>
#include "Subsystems/OCLTDebugSubsystem.h"
#include <Characters/OCLTBaseCharacter.h>
#include <OCLTGameInstance.h>

#include <Components/CapsuleComponent.h>
#include <GameFramework/Character.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

bool UOCLTPoolDetectorComponent::DetectPool()
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

/*#if ENABLE_DRAW_DEBUG
	UOCLTDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UOCLTDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else 
	bool bIsDebugEnabled = false;
#endif*/
	float DrawTime = 2.0f;
	FHitResult ForwardCheckHitResult;

	FVector LineStart = CachedCharacterOwner->GetActorLocation() + CachedCharacterOwner->GetActorForwardVector() * 200.0f;
	FVector LineEnd = LineStart - FVector(0.0f, 0.0f, 1000.0f);

	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Black, false, DrawTime);
	if (!GetWorld()->LineTraceSingleByObjectType(ForwardCheckHitResult, LineStart, LineEnd, UE_ECC_WATER_VOLUME, QueryParams))
	{
		//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Black, false, DrawTime);

		return false;
	}
	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, DrawTime);

	return true;
}

void UOCLTPoolDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("UOCLTLedgeDetectorComponent::BeginPlay() only a character can use UOCLTLedgeDetectorComponent"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}