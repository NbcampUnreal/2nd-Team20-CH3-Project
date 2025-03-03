#include "AI/BTDecorator_IsInAttackRange.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
    checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false."));

    ANXAIController* AIController = Cast<ANXAIController>(OwnerComp.GetAIOwner());
    checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

    ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIController->GetPawn());
    checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

    float AttackRange = NPC->GetAttackRange();

    ANXCharacterBase* TargetPlayerCharacter = Cast<ANXCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANXAIController::TargetCharacterKey));

    if (IsValid(TargetPlayerCharacter) == true && TargetPlayerCharacter->IsPlayerControlled() == true)
    {
        bool bInRange = NPC->GetDistanceTo(TargetPlayerCharacter) <= AttackRange;

        NPC->bIsChasing = !bInRange;
        NPC->bIsInAttackRange = bInRange; 

        if (NPC->GetMovementComponent())
        {
            UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
            if (MovementComponent)
            {
                MovementComponent->MaxWalkSpeed = bInRange ? NPC->GetSprintSpeed() : NPC->GetNormalSpeed();
            }
        }
        if (!bInRange)
        {
            AIController->MoveToActor(TargetPlayerCharacter, 100.0f, true, true, false);
        }
        else
        {
            AIController->MoveToActor(TargetPlayerCharacter, 0.0f, true, true, false);
        }

        return bInRange;
    }

    NPC->bIsChasing = false;
    NPC->bIsInAttackRange = false; 
    return false;
}
