#pragma once

#include "CoreMinimal.h"
#include "Gimmick/NXItem.h"
#include "NXDropItem.generated.h"

UCLASS()
class NXPROJECT_API ANXDropItem : public ANXItem
{
	GENERATED_BODY()

public:
	ANXDropItem();

protected:
	// 코인 획득 시 플레이어에게 줄 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	// 부모 클래스에서 상속받은 ActivateItem 함수를 오버라이드
	virtual void ActivateItem(AActor* Activator) override;




};