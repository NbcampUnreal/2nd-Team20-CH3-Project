#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NXAmmoWidget.generated.h"

class UTextBlock;

UCLASS()
class NXPROJECT_API UNXAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateAmmoDisplay(int32 CurrentAmmo, int32 MaxAmmo);

	void SetCurrentAmmoText(int32 amount);

	void SetMaxAmmoText(int32 amount);

	void HideAmmoText();

	void ShowAmmoText();

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmoText;

};
