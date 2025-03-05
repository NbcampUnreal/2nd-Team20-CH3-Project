#include "AI/BTService_DetectPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/NXCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    NodeName = TEXT("DetectPlayerCharacter");
    Interval = 1.f;
}

void UBTService_DetectPlayerCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ANXAIController* AIC = Cast<ANXAIController>(OwnerComp.GetAIOwner());
    if (!IsValid(AIC)) return;

    ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIC->GetPawn());
    if (!IsValid(NPC)) return;

    UWorld* World = NPC->GetWorld();
    if (!IsValid(World)) return;

    FVector CenterPosition = NPC->GetActorLocation();
    float DetectRadius = NPC->GetDetectRadius();
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);

    bool bResult = World->OverlapMultiByChannel(
        OverlapResults,
        CenterPosition,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(DetectRadius),
        CollisionQueryParams
    );

    if (bResult)
    {
        bool bPlayerDetected = false;
        ANXCharacterBase* DetectedPC = nullptr;

        for (const FOverlapResult& OverlapResult : OverlapResults)
        {
            ANXCharacterBase* PC = Cast<ANXCharacterBase>(OverlapResult.GetActor());
            if (!IsValid(PC)) continue;

            AController* Controller = PC->GetController();
            if (IsValid(Controller) && Controller->IsPlayerController())
            {
                DetectedPC = PC;
                bPlayerDetected = true;
                break;
            }
        }

        if (OwnerComp.GetBlackboardComponent())
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, DetectedPC);
        }

        UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
        if (MovementComponent)
        {
            MovementComponent->MaxWalkSpeed = bPlayerDetected ? NPC->GetSprintSpeed() : NPC->GetNormalSpeed();
        }
    }
    else if (OwnerComp.GetBlackboardComponent())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, nullptr);
    }

    if (ANXAIController::ShowAIDebug == 1)
    {
        DrawDebugSphere(World, CenterPosition, DetectRadius, 16, bResult ? FColor::Red : FColor::Green, false, 0.5f);
    }
}
