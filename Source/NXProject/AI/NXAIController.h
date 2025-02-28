#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NXAIController.generated.h"

class UBlackboardData;
class UbehaviorTree;

UCLASS()
class NXPROJECT_API ANXAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANXAIController();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void BeginAI(APawn* InPawn);
	void EndAI();

public:
	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;
	static const FName TargetCharacterKey;

	FTimerHandle PatrolTimerHandle = FTimerHandle();
	static int32 ShowAIDebug;
private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
