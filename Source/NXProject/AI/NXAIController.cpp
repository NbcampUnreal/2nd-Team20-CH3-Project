#include "AI/NXAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h" 

int32 ANXAIController::ShowAIDebug(0);
const FName ANXAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName ANXAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName ANXAIController::TargetCharacterKey(TEXT("TargetCharacter"));


ANXAIController::ANXAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));

}

void ANXAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
	}
}

void ANXAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndAI();

	Super::EndPlay(EndPlayReason);
}

void ANXAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent) == true)
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRunSucceeded == true, TEXT("Fail to run behavior tree."))
			
			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
		
			ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(InPawn);
			checkf(NPC != nullptr, TEXT("InPawn is not of type ANXNonPlayerCharacter"));

			float PatrolRadius = NPC->GetPatrolRadius();

			if (ShowAIDebug == 1)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI()")));
			}
		}
	}
}

void ANXAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();

		if (ShowAIDebug == 1)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EndAI()")));
		}
	}
}