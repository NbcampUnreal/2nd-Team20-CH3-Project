#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NXCrosshairHUD.generated.h"


UCLASS()
class NXPROJECT_API ANXCrosshairHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void DrawHUD() override;

public:
	UPROPERTY(EditAnywhere, Category = "Crosshair")
	float CrosshairSize = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	float SpreadMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Crosshair")
	FLinearColor CrosshairColor = FLinearColor::White;

	void UpdateCrosshair(float Speed, bool bIsAimingAtEnemy);
	
};
