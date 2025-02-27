// NXUIPlayerController.cpp

#include "UI/NXUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ANXUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (true == ::IsValid(UIWidgetClass))
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		// CreateWidget()이 호출될 때 UIWidgetInstance->NativeOnInitialize() 함수가 호출됨.
		if (true == ::IsValid(UIWidgetInstance))
		{
			UIWidgetInstance->AddToViewport();
			// AddToViewport()가 호출 될 때 UIWidgetInstance->NativeConstruct() 함수가 호출됨.

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}
}