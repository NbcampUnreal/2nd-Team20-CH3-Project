#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/NXItemInterface.h"  // ������ �������̽� ��� ����
#include "NXItem.generated.h"

UCLASS()
class NXPROJECT_API ANXItem : public AActor, public INXItemInterface
{
	GENERATED_BODY()

public:
	ANXItem();

protected:
	// ������ ����(Ÿ��)�� ���� �����ϰ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	// IItemInterface���� �䱸�ϴ� �Լ����� �ݵ�� ����
	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	// �������� �����ϴ� ���� �Լ� (�߰� ����Ʈ�� ������ ���� �� ����)
	virtual void DestroyItem();
};