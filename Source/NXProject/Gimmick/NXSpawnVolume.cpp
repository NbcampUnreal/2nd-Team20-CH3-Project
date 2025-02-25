#include "Gimmick/NXSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
ANXSpawnVolume::ANXSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    // �ڽ� ������Ʈ�� �����ϰ�, �� ������ ��Ʈ�� ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);

    ItemDataTable = nullptr;
}

void ANXSpawnVolume::SpawnRandomItem()
{
    if (FNXItemSpawnRow* SelectedRow = GetRandomItem())
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            SpawnItem(ActualClass);
        }
    }
}

FVector ANXSpawnVolume::GetRandomPointInVolume() const
{
    // 1) �ڽ� ������Ʈ�� �����ϵ� Extent, �� x/y/z �������� ������(���� ����)�� ����
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    // 2) �ڽ� �߽� ��ġ
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    // 3) �� �ະ�� -Extent ~ +Extent ������ ������ �� ����
    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

FNXItemSpawnRow* ANXSpawnVolume::GetRandomItem() const
{
    if (!ItemDataTable) return nullptr;

    // 1) ��� Row(��) ��������
    TArray<FNXItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));
    ItemDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    // 2) ��ü Ȯ�� �� ���ϱ�
    float TotalChance = 0.0f; // �ʱ�ȭ
    for (const FNXItemSpawnRow* Row : AllRows) // AllRows �迭�� �� Row�� ��ȸ
    {
        if (Row) // Row�� ��ȿ���� Ȯ��
        {
            TotalChance += Row->SpawnChance; // SpawnChance ���� TotalChance�� ���ϱ�
        }
    }


    // 3) 0 ~ TotalChance ���� ���� ��
    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    // 4) ���� Ȯ���� ������ ����
    for (FNXItemSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

void ANXSpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return;

    GetWorld()->SpawnActor<AActor>(
        ItemClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );
}