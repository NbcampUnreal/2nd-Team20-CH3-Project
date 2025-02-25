#include "AI/BTDecorator_IsInAttackRange.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false."));
	
	// Behavior Tree의 AIController 가져옴
	ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	// AIController가 소유하는 NPC 가져옴
	ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// AttackRange 가져옴
	float AttackRange = NPC->GetAttackRange();

	// Blackboard에서 목표로 설정된 PlayerCharacter를 가져옴.
	// AIController의 Blackboard에서 TargetCharacterKey로 저장된 객체를 가져옴.
	ANXCharacterBase* TargetPlayerCharacter = Cast<ANXCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANXAIController::TargetCharacterKey));
	
	// 목표 캐릭터가 유효하고, 플레이어가 직접 조종하는 캐릭터인지 확인.
	// NPC와 목표 플레이어 캐릭터 사이의 거리가 공격 범위 내에 있으면 true를 반환.
	if (IsValid(TargetPlayerCharacter) == true && TargetPlayerCharacter->IsPlayerControlled() == true)
	{
		bool bInRange = NPC->GetDistanceTo(TargetPlayerCharacter) <= AttackRange;

		if (bInRange)
		{
			// 공격 범위 내에 있을 때 이동 속도를 빠르게 설정
			if (NPC->GetMovementComponent())
			{
				UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
				if (MovementComponent)
				{
					MovementComponent->MaxWalkSpeed = NPC->GetSprintSpeed();
				}
			}
		}
		else
		{
			// 공격 범위 밖에 있을 때 기본 속도로 되돌리기
			if (NPC->GetMovementComponent())
			{
				UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
				if (MovementComponent)
				{
					MovementComponent->MaxWalkSpeed = NPC->GetNormalSpeed(); 
				}
			}
		}
		return bInRange;
	}
	return false;
}