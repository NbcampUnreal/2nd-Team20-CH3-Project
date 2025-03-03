#include "Player/NXAmmoWidget.h"
#include "Components/TextBlock.h"




void UNXAmmoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // UI가 생성될 때 기본값 설정
    if (CurrentAmmoText) CurrentAmmoText->SetText(FText::FromString(TEXT("30")));
    if (MaxAmmoText) MaxAmmoText->SetText(FText::FromString(TEXT("30")));
}

void UNXAmmoWidget::UpdateAmmoDisplay(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (CurrentAmmoText && MaxAmmoText)
    {
        CurrentAmmoText->SetText(FText::AsNumber(CurrentAmmo));
        MaxAmmoText->SetText(FText::AsNumber(MaxAmmo));
    }
}

void UNXAmmoWidget::SetCurrentAmmoText(int32 amount)
{
    if (CurrentAmmoText)
    {
        CurrentAmmoText->SetText(FText::AsNumber(amount));
    }
}

void UNXAmmoWidget::SetMaxAmmoText(int32 amount)
{
    if (MaxAmmoText)
    {
        MaxAmmoText->SetText(FText::AsNumber(amount));
    }
}

void UNXAmmoWidget::HideAmmoText()
{
    if (CurrentAmmoText && MaxAmmoText)
    {
        CurrentAmmoText->SetVisibility(ESlateVisibility::Hidden);
        MaxAmmoText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UNXAmmoWidget::ShowAmmoText()
{
    if (CurrentAmmoText && MaxAmmoText)
    {
        CurrentAmmoText->SetVisibility(ESlateVisibility::Visible);
        MaxAmmoText->SetVisibility(ESlateVisibility::Visible);
    }
}