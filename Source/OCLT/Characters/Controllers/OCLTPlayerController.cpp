// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/Controllers/OCLTPlayerController.h"
#include "Characters/OCLTBaseCharacter.h"

void AOCLTPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AOCLTPlayerController::ClimbLadderUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ClimbLadderUp(Value);
	}
}

void AOCLTPlayerController::InteractWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractWithLadder();
	}
}

void AOCLTPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AOCLTPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AOCLTPlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
	}
}
void AOCLTPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}
void AOCLTPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AOCLTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AOCLTPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AOCLTPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AOCLTPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AOCLTPlayerController::LookUp);
	InputComponent->BindAxis("SwimForward", this, &AOCLTPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AOCLTPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AOCLTPlayerController::SwimUp);
	InputComponent->BindAxis("ClimbLadderUp", this, &AOCLTPlayerController::ClimbLadderUp);
	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::InteractWithLadder);
	InputComponent->BindAction("SwimJump", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::SwimJump);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AOCLTPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AOCLTPlayerController::StopSprint);
	
}

void AOCLTPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBaseCharacter = Cast<AOCLTBaseCharacter>(InPawn);
}
void AOCLTPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}
void AOCLTPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}
void AOCLTPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}
void AOCLTPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AOCLTPlayerController::SwimJump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimJump();
	}
}

void AOCLTPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void AOCLTPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}
