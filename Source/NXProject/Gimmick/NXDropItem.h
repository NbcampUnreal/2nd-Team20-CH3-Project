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
	
	virtual void ActivateItem(AActor* Activator) override;

protected:
	// 코인 획득 시 얻을 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;






};