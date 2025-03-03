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
    Super::ActivateItem(Activator);

    ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(Activator);
    if (PlayerCharacter)
    {
        float CurrentHealth = PlayerCharacter->GetCurrentHealth();
        float MaxHealth = PlayerCharacter->GetMaxHealth();
        float NewHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);

        PlayerCharacter->SetHealth(NewHealth);

        UE_LOG(LogTemp, Log, TEXT("Healed for %d HP"), static_cast<int32>(HealAmount));

        DestroyItem();
    }
}