#include "Gimmick/NXSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ANXSpawnVolume::ANXSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    // 씬 컴포넌트 생성
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 스포닝 박스 컴포넌트 생성 및 부착
    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

void ANXSpawnVolume::BeginPlay()
{
    Super::BeginPlay();

    // 초기 아이템 스폰
    SpawnRandomItem();
}

void ANXSpawnVolume::SpawnRandomItem()
{
    // 먼저 이전 아이템 제거
    DestroyItem();

    // 스폰 가능한 아이템이 있는지 확인
    if (SpawnableItems.Num() > 0)
    {
        // 전체 확률 합 계산
        float TotalChance = 0.0f;
        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            TotalChance += ItemData.SpawnChance;
        }

        // 0 ~ 전체 확률 사이 랜덤 값 생성
        float RandomValue = FMath::FRandRange(0.0f, TotalChance);
        float AccumulatedChance = 0.0f;

        for (const FNXItemSpawnData& ItemData : SpawnableItems)
        {
            AccumulatedChance += ItemData.SpawnChance;

            // 랜덤 값이 누적 확률보다 작으면 해당 아이템 스폰
            if (RandomValue <= AccumulatedChance)
            {
                if (ItemData.ItemClass)
                {
                    FVector SpawnLocation = GetRandomPointInVolume();

                    // 아이템 스폰 및 CurrentSpawnedItem 업데이트
                    CurrentSpawnedItem = GetWorld()->SpawnActor<AActor>(
                        ItemData.ItemClass,
                        SpawnLocation,
                        FRotator::ZeroRotator
                    );

                    // 로그 출력
                    UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at Location: %s"),
                        *ItemData.ItemClass->GetName(),
                        *SpawnLocation.ToString());

                    break; // 한 개의 아이템만 스폰
                }
            }
        }
    }
}

FVector ANXSpawnVolume::GetRandomPointInVolume() const
{
    // 스포닝 박스의 익스텐트와 위치를 기반으로 랜덤 위치 생성
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
    // 이전에 스폰된 아이템이 유효하다면 제거
    if (IsValid(CurrentSpawnedItem))
    {
        CurrentSpawnedItem->Destroy();
        CurrentSpawnedItem = nullptr;
    }
}