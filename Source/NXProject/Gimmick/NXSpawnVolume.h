#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXItemSpawnRow.h"       
#include "NXSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class NXPROJECT_API ANXSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
    ANXSpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    // 스폰 영역을 담당할 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    // 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnRandomItem();

    FVector GetRandomPointInVolume() const;
    FNXItemSpawnRow* GetRandomItem() const;
    void SpawnItem(TSubclassOf<AActor> ItemClass);
   
};
