#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NXItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNXItemInterface : public UInterface
{
    GENERATED_BODY()
};

class NXPROJECT_API INXItemInterface
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) = 0;

    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) = 0;

    virtual void ActivateItem(AActor* Activator) = 0;
    virtual FName GetItemType() const = 0;
};
