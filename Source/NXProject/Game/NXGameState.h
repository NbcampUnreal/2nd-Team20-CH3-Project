#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NXGameState.generated.h"


UCLASS()
class NXPROJECT_API ANXGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	ANXGameState();

protected:

	virtual void BeginPlay() override;

public:

    void UpdateHUD();
private:

	FTimerHandle HUDUpdateTimerHandle;
};
