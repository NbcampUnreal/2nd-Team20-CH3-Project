#include "NXGameMode.h"
#include "NXProject/Player/NXPlayerCharacter.h"
#include "NXProject/Player/NXPlayerController.h"

ANXGameMode::ANXGameMode()
{
	DefaultPawnClass = ANXCharacterBase::StaticClass();
	PlayerControllerClass = ANXPlayerController::StaticClass();
}