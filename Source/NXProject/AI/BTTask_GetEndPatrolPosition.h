#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetEndPatrolPosition.generated.h"

UCLASS()
class NXPROJECT_API UBTTask_GetEndPatrolPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetEndPatrolPosition();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
