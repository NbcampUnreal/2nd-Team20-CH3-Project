#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/NXAnimInstance.h"


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	:bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	// 레벨에 배치되거나 새로 생성될시 NXAIController 빙의 자동 진행
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANXNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void ANXNonPlayerCharacter::BeginAttack()
{
	UNXAnimInstance* AnimInstance = Cast<UNXAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstnace"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMontage);

		bIsNowAttacking = true;

		if (OnAttackMontageEndedDelegate.IsBound() == false)
		{
			OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
		}
	}
}

void ANXNonPlayerCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsNowAttacking = false;

	if (OnAttackMontageEndedDelegate.IsBound() == true)
	{
		OnAttackMontageEndedDelegate.Unbind();
	}
}
