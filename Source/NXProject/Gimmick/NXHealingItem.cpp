#include "Gimmick/NXHealingItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Engine/Engine.h" 

ANXHealingItem::ANXHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void ANXHealingItem::ActivateItem(AActor* Activator)
{
    ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(Activator);
    if (PlayerCharacter)
    {
        // 현재 체력 + 회복량, 최대 체력을 넘지 않도록 처리
        float CurrentHealth = PlayerCharacter->GetCurrentHealth();
        float MaxHealth = PlayerCharacter->GetMaxHealth();
        float NewHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);

        PlayerCharacter->SetHealth(NewHealth);

        // 디버그 메시지 또는 UI 알림
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            FString::Printf(TEXT("Healed for %d HP"), HealAmount));

        // 아이템 제거
        DestroyItem();
    }
}