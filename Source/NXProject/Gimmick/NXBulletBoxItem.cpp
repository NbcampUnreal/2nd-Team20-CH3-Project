#include "Gimmick/NXBulletBoxItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Player/NXWeaponRifle.h"

ANXBulletBoxItem::ANXBulletBoxItem()
{
}

void ANXBulletBoxItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(Activator);
    if (PlayerCharacter)
    {
        ANXWeaponRifle* CurrentWeapon = PlayerCharacter->GetWeaponActor();
        if (CurrentWeapon)
        {
            CurrentWeapon->AddAmmo(AmmoAmount);

            DestroyItem();
        }
    }
}