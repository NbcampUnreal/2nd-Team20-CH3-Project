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
	// ���� ȹ�� �� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;






};