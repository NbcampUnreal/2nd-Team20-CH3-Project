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
	// ������ ��ġ�ǰų� ���� �����ɽ� NXAIController ���� �ڵ� ����
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
		GetOverlappingActors(OverlappingActors);  // ���� ���� ĳ���͵��� ������

		for (AActor* Actor : OverlappingActors)
		{
			// ���� ���� ����� ���̶�� ���ظ� ����
			ANXCharacterBase* TargetCharacter = Cast<ANXCharacterBase>(Actor);
			if (TargetCharacter && TargetCharacter != this)  // �ڱ� �ڽ��� ����
			{

				float DamageAmount = GetAttackDamage();  // �������� ���ݷ�

				// FPointDamageEvent ��ü ����
                FHitResult HitResult;
                FPointDamageEvent DamageEvent(DamageAmount, HitResult, FVector::ZeroVector, nullptr);
				// ���ظ� ��󿡰� ����
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











