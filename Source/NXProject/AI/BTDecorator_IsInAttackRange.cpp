#include "AI/BTDecorator_IsInAttackRange.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false."));
	
	// Behavior Tree�� AIController ��������
	ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	// AIController�� �����ϴ� NPC��������
	ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// AttackRange ��������
	float AttackRange = NPC->GetAttackRange();

	// Blackboard���� ��ǥ PlyaerCharacter �������� -> �̰��� AIController���� ���ǵ� Ű�� ���� ����.
	ANXCharacterBase* TargetPlayerCharacter = Cast<ANXCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANXAIController::TargetCharacterKey));
	// ��ǥ �÷��̾ ��ȿ�ϰ� �÷��̾ �����ϴ� ĳ������ ���, NPC�� �÷��̾� ������ �Ÿ��� ���.
	// -> �Ÿ��� AttackRange�̶�� ���� ���� ���¶�� �Ǵ�.
	if (IsValid(TargetPlayerCharacter) == true && TargetPlayerCharacter->IsPlayerControlled() == true)
	{
		return NPC->GetDistanceTo(TargetPlayerCharacter) <= AttackRange;
	}

	return false;
}