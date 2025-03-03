#include "AI/BTTask_Attack.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXPlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
    ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());

    if (!AIController || !NPC)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (Player)
    {
        float DistanceToPlayer = FVector::Dist(NPC->GetActorLocation(), Player->GetActorLocation());

        if (DistanceToPlayer > NPC->GetAttackRange())
        {
            if (NPC->bIsNowAttacking)
            {
                AIController->MoveToActor(Player, NPC->GetAttackRange(), false, true, false);
            }
        }
    }

    if (!NPC->bIsNowAttacking)
    {
        NPC->bIsAttacking = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	NPC->bIsAttacking = true;
	NPC->BeginAttack();

	return EBTNodeResult::InProgress;

}
