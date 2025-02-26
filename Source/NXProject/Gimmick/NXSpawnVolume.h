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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<FNXItemSpawnData> SpawnableItems;

    UPROPERTY()
    AActor* CurrentSpawnedItem;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnRandomItem();

    UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetRandomPointInVolume() const;

private:
    void DestroyItem();
};