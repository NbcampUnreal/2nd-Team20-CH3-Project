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
	// ���� ����(1��)
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
				// NPC�� ���� ��ġ
				FVector CenterPosition = NPC->GetActorLocation();
				// Ž�� ����
				float DetectRadius = 300.f;
				TArray<FOverlapResult> OverlapResults;
				// NPC�� �����ϴ� �浹 ����
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				// ������ ���� ������ �浹�ϴ� ��� ���� ������
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					CenterPosition,
					FQuat::Identity, // ȸ�� ������ ��� X
					ECollisionChannel::ECC_GameTraceChannel2, // �浹 ä�� 
					FCollisionShape::MakeSphere(DetectRadius), // ��ü�� Ž�� ����
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
								// -- Ž�� �� �ð�ȭ --
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