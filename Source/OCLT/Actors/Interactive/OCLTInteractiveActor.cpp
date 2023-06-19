// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactive/OCLTInteractiveActor.h"
#include <Characters/OCLTBaseCharacter.h>

#include <Components/CapsuleComponent.h>


void AOCLTInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOCLTInteractiveActor::OnInteractionVolumeOverlapBegin);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AOCLTInteractiveActor::OnInteractionVolumeOverlapEnd);
	}
}

void AOCLTInteractiveActor::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	AOCLTBaseCharacter* OCLTBaseCharacter = StaticCast<AOCLTBaseCharacter*>(OtherActor);
	OCLTBaseCharacter->RegisterInteractiveActor(this);
}

bool AOCLTInteractiveActor::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	AOCLTBaseCharacter* BaseCharacter = Cast<AOCLTBaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return false;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return false;
	}
	return true;
}

void AOCLTInteractiveActor::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	AOCLTBaseCharacter* OCLTBaseCharacter = StaticCast<AOCLTBaseCharacter*>(OtherActor);
	OCLTBaseCharacter->UnregisterInteractiveActor(this);
}

