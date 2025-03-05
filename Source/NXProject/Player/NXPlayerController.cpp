#include "Player/NXPlayerController.h"
#include "Player/NXCharacterBase.h"
#include "Player/NXPlayerCharacter.h"
#include "Game/NXGameState.h"
#include "Components/TextBlock.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Player/NXWeaponRifle.h"

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
	QuickSlot02(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	GameOverWidgetClass(nullptr),
	GameOverWidgetInstance(nullptr)
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

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	ANXGameState* NXGameState = GetWorld() ? GetWorld()->GetGameState<ANXGameState>() : nullptr;
	if (NXGameState)
	{
		NXGameState->UpdateHUD();
	}

	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();
			GameOverWidgetInstance->SetVisibility(ESlateVisibility::Hidden);  // 처음에는 숨김
		}
	}

}

UUserWidget* ANXPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ANXPlayerController::SetHUDVisibility(bool bIsVisible)
{
	if (GetHUDWidget()) 
	{
		GetHUDWidget()->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ANXPlayerController::SetupInputComponent()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
	
	}
}

void ANXPlayerController::ShowGameOverWidget(bool bIsVisible)
{
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		// 입력 비활성화 (UI 전용 모드)
		if (bIsVisible)
		{
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
			UE_LOG(LogTemp, Warning, TEXT("Game Over UI가 화면에 표시됨"));
		}
		else
		{
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[ShowGameOverWidget] GameOverWidgetInstance가 NULL!"));
	}
}