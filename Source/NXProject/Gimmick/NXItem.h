#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/NXItemInterface.h"  // 만들어둔 인터페이스 헤더 포함
#include "NXItem.generated.h"

UCLASS()
class NXPROJECT_API ANXItem : public AActor, public INXItemInterface
{
	GENERATED_BODY()

public:
	ANXItem();

protected:
	// 아이템 유형(타입)을 편집 가능하게 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	// IItemInterface에서 요구하는 함수들을 반드시 구현
	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	// 아이템을 제거하는 공통 함수 (추가 이펙트나 로직을 넣을 수 있음)
	virtual void DestroyItem();
};