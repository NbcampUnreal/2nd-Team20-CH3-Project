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
        // ���� ü�� + ȸ����, �ִ� ü���� ���� �ʵ��� ó��
        float CurrentHealth = PlayerCharacter->GetCurrentHealth();
        float MaxHealth = PlayerCharacter->GetMaxHealth();
        float NewHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);

        PlayerCharacter->SetHealth(NewHealth);

        // ����� �޽��� �Ǵ� UI �˸�
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            FString::Printf(TEXT("Healed for %d HP"), HealAmount));

        // ������ ����
        DestroyItem();
    }
}