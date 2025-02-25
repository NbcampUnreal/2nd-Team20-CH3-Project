#include "Gimmick/NXSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ANXSpawnVolume::ANXSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    // �� ������Ʈ ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // ������ �ڽ� ������Ʈ ���� �� ����
    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

void ANXSpawnVolume::BeginPlay()
{
    Super::BeginPlay();

    // �ʱ� ������ ����
    SpawnRandomItem();
}

void ANXSpawnVolume::SpawnRandomItem()
{
    // ���� ���� ������ ����
    DestroyItem();

    // ���� ������ �������� �ִ��� Ȯ��
    if (SpawnableItems.Num() > 0)
    {
        // ��ü Ȯ�� �� ���
        float TotalChance = 0.0f;
        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            TotalChance += ItemData.SpawnChance;
        }

        // 0 ~ ��ü Ȯ�� ���� ���� �� ����
        float RandomValue = FMath::FRandRange(0.0f, TotalChance);
        float AccumulatedChance = 0.0f;

        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            AccumulatedChance += ItemData.SpawnChance;

            // ���� ���� ���� Ȯ������ ������ �ش� ������ ����
            if (RandomValue <= AccumulatedChance)
            {
                if (ItemData.ItemClass)
                {
                    FVector SpawnLocation = GetRandomPointInVolume();

                    // ������ ���� �� CurrentSpawnedItem ������Ʈ
                    CurrentSpawnedItem = GetWorld()->SpawnActor<AActor>(
                        ItemData.ItemClass,
                        SpawnLocation,
                        FRotator::ZeroRotator
                    );

                    // �α� ���
                    UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at Location: %s"),
                        *ItemData.ItemClass->GetName(),
                        *SpawnLocation.ToString());

                    break; // �� ���� �����۸� ����
                }
            }
        }
    }
}

FVector ANXSpawnVolume::GetRandomPointInVolume() const
{
    // ������ �ڽ��� �ͽ���Ʈ�� ��ġ�� ������� ���� ��ġ ����
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
    // ������ ������ �������� ��ȿ�ϴٸ� ����
    if (IsValid(CurrentSpawnedItem))
    {
        CurrentSpawnedItem->Destroy();
        CurrentSpawnedItem = nullptr;
    }
}