#include "Characters/Animations/OCLTBaseCharacterAnimInstance.h" //этот инклюд всегда должен быть первым

//#include "OCLT/../.h"
#include "Characters/OCLTBaseCharacter.h"
#include "Components/MovementComponents/OCLTCharacterMovementComponent.h"

void UOCLTBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AOCLTBaseCharacter>(), TEXT("UOCLTBaseCharacterAnimInstance::NativeBeginPlay() UOCLTBaseCharacterAnimInstance can be used only with AOCLTBaseCharacter"));
	CachedBaseCharacter = StaticCast<AOCLTBaseCharacter*>(TryGetPawnOwner()); //TryGetPawnOwner преобразовываем в AOCLTBaseCharacter (из TryGetPawnOwner получаем AOCLTBaseCharacter)

}

void UOCLTBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UOCLTCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetOCLTCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size(); //к Speed подключаем скорость из UCharacterMovementComponent
	bIsFalling = CharacterMovement->IsFalling(); //к bIsFalling подключаем положение из UCharacterMovementComponent
	bIsCrouching = CharacterMovement->IsCrouching(); //к bIsCrouching подключаем положение из UCharacterMovementComponent
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsSwimming = CharacterMovement->IsSwimming();
	bIsOnLadder = CharacterMovement->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovement->GetLadderRatioSpeed();
	}
}
