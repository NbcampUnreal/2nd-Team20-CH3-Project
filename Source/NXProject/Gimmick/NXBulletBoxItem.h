#pragma once

#include "CoreMinimal.h"
#include "Gimmick/NXItem.h"
#include "NXBulletBoxItem.generated.h"

UCLASS()
class NXPROJECT_API ANXBulletBoxItem : public ANXItem
{
    GENERATED_BODY()

public:
    ANXBulletBoxItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoAmount = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* BulletBoxItemSound;

protected:

    virtual void ActivateItem(AActor* Activator) override;
};
