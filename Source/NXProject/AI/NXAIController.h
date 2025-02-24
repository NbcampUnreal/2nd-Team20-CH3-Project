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

	// -- AI 시작 종료시 --
	void BeginAI(APawn* InPawn);
	void EndAI();

public:
	// -- Blackbord 변수 --
	// 순찰 시작 위치
	static const FName StartPatrolPositionKey;
	// 순찰 종료 위치
	static const FName EndPatrolPositionKey;
	// 추격 타켓 지정
	static const FName TargetCharacterKey;

	// 순찰을 할 일정 시간 반복을 위한 타이머
	FTimerHandle PatrolTimerHandle = FTimerHandle();
	
	// 디버깅 활성화용
	static int32 ShowAIDebug;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
