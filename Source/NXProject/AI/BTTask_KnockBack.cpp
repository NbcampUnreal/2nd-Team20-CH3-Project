#include "AI/BTTask_KnockBack.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTTask_KnockBack::UBTTask_KnockBack()
	: KnockBackStrength(100.f)
{
	bNotifyTick = true;
	bHasKnockedBack = false;
}

EBTNodeResult::Type UBTTask_KnockBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
	if (!NPC)
	{
		return EBTNodeResult::Failed;
	}

	AActor* DamageCauser = NPC->GetDamageSource();
	if (DamageCauser)
	{
		KnockBackDirection = NPC->GetActorLocation() - DamageCauser->GetActorLocation();
		KnockBackDirection.Normalize();  
	}

	NPC->ApplyKnockBack(KnockBackDirection, KnockBackStrength);

	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsKnockedBack"), true);

	bHasKnockedBack = true;*/

	return EBTNodeResult::InProgress;
}

void UBTTask_KnockBack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	/*ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
	if (!NPC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (bHasKnockedBack)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsKnockedBack"), false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}*/
}

