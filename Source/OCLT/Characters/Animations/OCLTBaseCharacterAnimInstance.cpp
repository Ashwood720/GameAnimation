#include "Characters/Animations/OCLTBaseCharacterAnimInstance.h" //���� ������ ������ ������ ���� ������

//#include "OCLT/../.h"
#include "Characters/OCLTBaseCharacter.h"
#include "Components/MovementComponents/OCLTCharacterMovementComponent.h"

void UOCLTBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AOCLTBaseCharacter>(), TEXT("UOCLTBaseCharacterAnimInstance::NativeBeginPlay() UOCLTBaseCharacterAnimInstance can be used only with AOCLTBaseCharacter"));
	CachedBaseCharacter = StaticCast<AOCLTBaseCharacter*>(TryGetPawnOwner()); //TryGetPawnOwner ��������������� � AOCLTBaseCharacter (�� TryGetPawnOwner �������� AOCLTBaseCharacter)

}

void UOCLTBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UOCLTCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetOCLTCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size(); //� Speed ���������� �������� �� UCharacterMovementComponent
	bIsFalling = CharacterMovement->IsFalling(); //� bIsFalling ���������� ��������� �� UCharacterMovementComponent
	bIsCrouching = CharacterMovement->IsCrouching(); //� bIsCrouching ���������� ��������� �� UCharacterMovementComponent
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsSwimming = CharacterMovement->IsSwimming();
	bIsOnLadder = CharacterMovement->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovement->GetLadderRatioSpeed();
	}
}
