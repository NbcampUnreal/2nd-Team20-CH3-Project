#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/NXItemInterface.h" 
#include "NXItem.generated.h"

class USphereComponent;

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

	// ��Ʈ ������Ʈ (��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	// �浹 ������Ʈ (�÷��̾� ���� ���� ������)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	// ������ �ð� ǥ���� ����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;

	// IItemInterface���� �䱸�ϴ� �Լ����� �ݵ�� ����
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	// �������� �����ϴ� ���� �Լ� (�߰� ����Ʈ�� ������ ���� �� ����)
	virtual void DestroyItem();
};