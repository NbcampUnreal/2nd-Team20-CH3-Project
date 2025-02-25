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
	// ���� ȹ�� �� �÷��̾�� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	// �θ� Ŭ�������� ��ӹ��� ActivateItem �Լ��� �������̵�
	virtual void ActivateItem(AActor* Activator) override;




};