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

	// -- AI ���� ����� --
	void BeginAI(APawn* InPawn);
	void EndAI();

public:
	// -- Blackbord ���� --
	// ���� ���� ��ġ
	static const FName StartPatrolPositionKey;
	// ���� ���� ��ġ
	static const FName EndPatrolPositionKey;
	// �߰� Ÿ�� ����
	static const FName TargetCharacterKey;

	// ������ �� ���� �ð� �ݺ��� ���� Ÿ�̸�
	FTimerHandle PatrolTimerHandle = FTimerHandle();
	
	// ����� Ȱ��ȭ��
	static int32 ShowAIDebug;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
