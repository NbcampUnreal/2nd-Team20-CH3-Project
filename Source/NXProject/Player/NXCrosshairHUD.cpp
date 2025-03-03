#include "Player/NXCrosshairHUD.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

void ANXCrosshairHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas) return;

    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    float CurrentCrosshairSize = CrosshairSize * SpreadMultiplier;
    FLinearColor CurrentColor = CrosshairColor;

    float LineThickness = 2.0f;
    float Gap = 5.0f + (CurrentCrosshairSize * 0.5f);

    DrawLine(CenterX, CenterY - Gap, CenterX, CenterY - Gap - CurrentCrosshairSize, CurrentColor, LineThickness);

    DrawLine(CenterX, CenterY + Gap, CenterX, CenterY + Gap + CurrentCrosshairSize, CurrentColor, LineThickness);

    DrawLine(CenterX - Gap, CenterY, CenterX - Gap - CurrentCrosshairSize, CenterY, CurrentColor, LineThickness);

    DrawLine(CenterX + Gap, CenterY, CenterX + Gap + CurrentCrosshairSize, CenterY, CurrentColor, LineThickness);
}

void ANXCrosshairHUD::UpdateCrosshair(float Speed, bool bIsAimingAtEnemy)
{
    float SpeedFactor = FMath::Clamp(Speed / 600.0f, 0.0f, 1.0f);
    SpreadMultiplier = FMath::Lerp(1.0f, 2.0f, SpeedFactor);

    CrosshairColor = bIsAimingAtEnemy ? FLinearColor::Red : FLinearColor::White;
}
