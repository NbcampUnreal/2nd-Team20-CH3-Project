#include "Gimmick/NXDropItem.h"

ANXDropItem::ANXDropItem()
{
	PointValue = 0;
	ItemType = "DefaultDropItem";
}

void ANXDropItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));

        DestroyItem();
    }
}
