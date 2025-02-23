#include "AI/BTService_DetectPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "AI/NXNonPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/NXCharacterBase.h"
#include "Engine/OverlapResult.h"

UBTService_DetectPlayerCharacter::UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NodeName = TEXT("DetectPlayerCharacter");
	// 실행 간격(1초)
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
				// NPC의 현재 위치
				FVector CenterPosition = NPC->GetActorLocation();
				// 탐지 범위
				float DetectRadius = 300.f;
				TArray<FOverlapResult> OverlapResults;
				// NPC를 무시하는 충돌 쿼리
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// 지정된 범위 내에서 충돌하는 모든 액터 ㅊㅈ기
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity, // 회전 정보는 사용 X
					ECollisionChannel::ECC_GameTraceChannel2, // 충돌 채널 
					FCollisionShape::MakeSphere(DetectRadius), // 구체로 탐지 범위
					CollisionQueryParams
				);

				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{
						ANXCharacterBase* PC = Cast<ANXCharacterBase>(OverlapResult.GetActor());
						if (IsValid(PC) == true && PC->GetController()->IsPlayerController() == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, PC);

							if (ANXAIController::ShowAIDebug == 1)
							{
								// -- 탐지 시 시각화 --
								UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Detected!")));
								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Red, false, 0.5f);
								DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								DrawDebugLine(World, NPC->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
							}
							break;
						}
						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANXAIController::TargetCharacterKey, nullptr);
							
							if (ANXAIController::ShowAIDebug == 1)
							{
								DrawDebugSphere(World, CenterPosition, DetectRadius, 16, FColor::Green, false, 0.5f);
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