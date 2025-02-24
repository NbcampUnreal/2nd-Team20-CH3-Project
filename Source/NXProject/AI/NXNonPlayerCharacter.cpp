#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/NXAnimInstance.h"
#include "Engine/DamageEvents.h"



ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	: bIsNowAttacking(false)
	, PatrolRadius(200)
	, DetectRadius(100)
	, AttackRange(50)

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

		// ??
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);  // 범위 내의 캐릭터들을 가져옴

		for (AActor* Actor : OverlappingActors)
		{
			// 범위 내의 대상이 적이라면 피해를 입힘
			ANXCharacterBase* TargetCharacter = Cast<ANXCharacterBase>(Actor);
			if (TargetCharacter && TargetCharacter != this)  // 자기 자신은 제외
			{

				float DamageAmount = GetAttackDamage();  // 공격자의 공격력

				// FPointDamageEvent 객체 생성
                FHitResult HitResult;
                FPointDamageEvent DamageEvent(DamageAmount, HitResult, FVector::ZeroVector, nullptr);
				// 피해를 대상에게 전달
				TargetCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
			}
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

float ANXNonPlayerCharacter::GetPatrolRadius() const
{
	return PatrolRadius;
}

float ANXNonPlayerCharacter::GetDetectRadius() const
{
	return DetectRadius;
}
float ANXNonPlayerCharacter::GetAttackRange() const
{
	return AttackRange;
}











