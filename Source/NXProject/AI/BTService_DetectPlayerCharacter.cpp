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
    if (IsValid(AIC) == true)
    {
        ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(AIC->GetPawn());
        if (IsValid(NPC) == true)
        {
            UWorld* World = NPC->GetWorld();
            if (IsValid(World) == true)
            {
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

                if (bResult == true)
                {
                    bool bPlayerDetected = false;

                    for (auto const& OverlapResult : OverlapResults)
                    {
                        ANXCharacterBase* PC = Cast<ANXCharacterBase>(OverlapResult.GetActor());
                        if (IsValid(PC) == true && PC->GetController()->IsPlayerController() == true)
                        {
                            OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, PC);
                            bPlayerDetected = true;

                            if (ANXAIController::ShowAIDebug == 1)
                            {
                                UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
                                DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
                                DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
                                DrawDebugLine(World, NPC->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
                            }
                            break;
                        }
                    }

                    if (bPlayerDetected)
                    {
                        if (NPC->GetMovementComponent())
                        {
                            UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
                            if (MovementComponent)
                            {
                                MovementComponent->MaxWalkSpeed = NPC->GetSprintSpeed(); 
                            }
                        }
                    }
                    else
                    {
                        if (NPC->GetMovementComponent())
                        {
                            UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(NPC->GetMovementComponent());
                            if (MovementComponent)
                            {
                                MovementComponent->MaxWalkSpeed = NPC->GetNormalSpeed(); 
                            }
                        }
                    }
                }
                else
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, nullptr);
                }

                if (ANXAIController::ShowAIDebug == 1)
                {
                    DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
                }
            }
        }
    }
}
