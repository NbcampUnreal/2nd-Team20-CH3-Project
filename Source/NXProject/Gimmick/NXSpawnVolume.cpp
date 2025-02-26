#include "Gimmick/NXSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ANXSpawnVolume::ANXSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

void ANXSpawnVolume::BeginPlay()
{
    Super::BeginPlay();

    SpawnRandomItem();
}

void ANXSpawnVolume::SpawnRandomItem()
{
    DestroyItem();

    if (SpawnableItems.Num() > 0)
    {
        float TotalChance = 0.0f;
        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            TotalChance += ItemData.SpawnChance;
        }

        float RandomValue = FMath::FRandRange(0.0f, TotalChance);
        float AccumulatedChance = 0.0f;

        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            AccumulatedChance += ItemData.SpawnChance;

            if (RandomValue <= AccumulatedChance)
            {
                if (ItemData.ItemClass)
                {
                    FVector SpawnLocation = GetRandomPointInVolume();

                    CurrentSpawnedItem = GetWorld()->SpawnActor<AActor>(
                        ItemData.ItemClass,
                        SpawnLocation,
                        FRotator::ZeroRotator
                    );

                    UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at Location: %s"),
                        *ItemData.ItemClass->GetName(),
                        *SpawnLocation.ToString());

                    break; 
                }
            }
        }
    }
}

FVector ANXSpawnVolume::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

void ANXSpawnVolume::DestroyItem()
{
    if (IsValid(CurrentSpawnedItem))
    {
        CurrentSpawnedItem->Destroy();
        CurrentSpawnedItem = nullptr;
    }
}