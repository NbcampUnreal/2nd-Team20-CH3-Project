#pragma once

#include "CoreMinimal.h"
#include "Gimmick/NXItem.h"
#include "NXHealingItem.generated.h"

UCLASS()
class NXPROJECT_API ANXHealingItem : public ANXItem
{
	GENERATED_BODY()

public:
	ANXHealingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
