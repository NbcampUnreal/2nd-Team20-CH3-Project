#include "NXGameMode.h"
#include "NXProject/Player/NXPlayerCharacter.h"
#include "NXProject/Player/NXPlayerController.h"
#include "Player/NXCrosshairHUD.h"

ANXGameMode::ANXGameMode()
{
	DefaultPawnClass = ANXCharacterBase::StaticClass();
	PlayerControllerClass = ANXPlayerController::StaticClass();
	HUDClass = ANXCrosshairHUD::StaticClass();
}