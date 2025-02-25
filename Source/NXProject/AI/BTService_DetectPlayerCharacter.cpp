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
                // NPC 위치 탐색
                FVector CenterPosition = NPC->GetActorLocation();

                // 탐지 범위
                float DetectRadius = NPC->GetDetectRadius();

                TArray<FOverlapResult> OverlapResults;

                // NPC 주변의 겹치는 영역 탐색.
                FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);

                // 겹치는 대상이 있는지 확인
                bool bResult = World->OverlapMultiByChannel(
                    OverlapResults,
                    CenterPosition,
                    FQuat::Identity, // 회전 없음
                    ECollisionChannel::ECC_GameTraceChannel2, // 해당 채널
                    FCollisionShape::MakeSphere(DetectRadius), // 구 형태의 탐지 범위
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
                                // 디버깅 정보
                                UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
                                DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
                                DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
                                DrawDebugLine(World, NPC->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
                            }
                            break;
                        }
                    }

                    // 플레이어가 탐지된 경우 속도 변경
                    if (bPlayerDetected)
                    {
                        // 이동 속도를 빠르게 설정
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
                        // 플레이어가 탐지되지 않은 경우 기본 속도
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
