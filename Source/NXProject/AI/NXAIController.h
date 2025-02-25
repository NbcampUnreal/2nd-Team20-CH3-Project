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

	// -- AI 시작 종료 --
	void BeginAI(APawn* InPawn);
	void EndAI();

public:
	// -- Blackbord 함수 --
	// 순찰 시작
	static const FName StartPatrolPositionKey;
	// 순찰 종료
	static const FName EndPatrolPositionKey;
	// 목표 타겟
	static const FName TargetCharacterKey;

	// 
	FTimerHandle PatrolTimerHandle = FTimerHandle();
	
	// 디버그 출력용
	static int32 ShowAIDebug;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
