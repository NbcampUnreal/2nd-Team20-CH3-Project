#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NXItemInterface.generated.h"

// �������̽��� UObject �ý��ۿ��� ����ϱ� ���� �⺻ ��ũ��
UINTERFACE(MinimalAPI)
class UNXItemInterface : public UInterface
{
    GENERATED_BODY()
};

// ���� C++ �������� ����� �Լ� ����(�ñ״�ó)�� ����
class NXPROJECT_API INXItemInterface
{
    GENERATED_BODY()

public:
    // �÷��̾ �� �������� ������ ������ �� ȣ��
    UFUNCTION()
    virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) = 0;

    // �÷��̾ �� �������� ������ ����� �� ȣ��
    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex) = 0;

    // �������� ���Ǿ��� �� ȣ��
    virtual void ActivateItem(AActor* Activator) = 0;
    // �� �������� ����(Ÿ��)�� ��ȯ (��: "Coin", "Mine" ��)
    virtual FName GetItemType() const = 0;
};
