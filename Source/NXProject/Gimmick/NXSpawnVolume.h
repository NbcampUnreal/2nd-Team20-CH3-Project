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
    // ���� ������ ����� �ڽ� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    // ���� ���� ���ο��� ������ ��ǥ�� ������ �Լ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnRandomItem();

    FVector GetRandomPointInVolume() const;
    FNXItemSpawnRow* GetRandomItem() const;
    void SpawnItem(TSubclassOf<AActor> ItemClass);
   
};
