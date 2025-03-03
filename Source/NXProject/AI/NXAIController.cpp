#include "AI/NXAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXPlayerCharacter.h"
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

void ANXAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ANXNonPlayerCharacter* AICharacter = Cast<ANXNonPlayerCharacter>(GetPawn());
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (AICharacter && Player)
	{
		float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), Player->GetActorLocation());

		if (DistanceToPlayer < AICharacter->GetAttackRange())
		{
			AICharacter->BeginAttack();

			GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackRange"), true);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackRange"), false);
		}
	}
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