#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXSpawnVolume.generated.h"

USTRUCT(BlueprintType)
struct FNXItemSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float SpawnChance = 33.33f;
};

class UBoxComponent;

UCLASS()
class NXPROJECT_API ANXSpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ANXSpawnVolume();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    // 스폰 가능한 아이템 목록
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<FNXItemSpawnData> SpawnableItems;

    // 현재 스폰된 아이템
    UPROPERTY()
    AActor* CurrentSpawnedItem;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnRandomItem();

    // 랜덤 위치 생성
    UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetRandomPointInVolume() const;

private:
    // 아이템 제거 함수
    void DestroyItem();
};