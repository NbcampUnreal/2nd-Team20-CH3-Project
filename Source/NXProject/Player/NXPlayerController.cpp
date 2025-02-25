#include "Player/NXPlayerController.h"
#include "EnhancedInputSubsystems.h"

ANXPlayerController::ANXPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	AttackAction(nullptr),
	PunchAction(nullptr),
	CrouchAction(nullptr),
	ReloadAction(nullptr),
	QuickSlot01(nullptr),
	QuickSlot02(nullptr)
{
}

void ANXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}