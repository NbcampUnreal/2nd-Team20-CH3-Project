#include "Game/NXGameState.h"
#include "Player/NXPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ANXGameState::ANXGameState()
{

}

void ANXGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ANXGameState::UpdateHUD,
		0.1f,
		true
	);
}

void ANXGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ANXPlayerController* NXPlayerController = Cast<ANXPlayerController>(PlayerController);
		if (NXPlayerController)
		{
			UUserWidget* HUDWidget = NXPlayerController->GetHUDWidget(); 
			if (HUDWidget)
			{
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UpdateHUD: NXPlayerController 캐스팅 실패"));
		}
	}
}