#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

UCLASS()
class NXPROJECT_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInAttackRange();

protected:
	// 공격 범위 내에 있는지 확인(BT에서 데코레이터가 사용되는 조건 계산)
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
