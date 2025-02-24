#include "Gimmick/NXHealingItem.h"

ANXHealingItem::ANXHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void ANXHealingItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // 회복 디버그 메시지
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP!"), HealAmount));

        DestroyItem();
    }
}