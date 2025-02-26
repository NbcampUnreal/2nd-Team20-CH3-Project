#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KnockBack.generated.h"


UCLASS()
class NXPROJECT_API UBTTask_KnockBack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KnockBack();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KnockBack")
	float KnockBackStrength;
	FVector KnockBackDirection;
	bool bHasKnockedBack;
	
};
