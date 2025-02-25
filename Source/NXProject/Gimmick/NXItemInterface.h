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
    virtual void OnItemOverlap(AActor* OverlapActor) = 0;
    // �÷��̾ �� �������� ������ ����� �� ȣ��
    virtual void OnItemEndOverlap(AActor* OverlapActor) = 0;
    // �������� ���Ǿ��� �� ȣ��
    virtual void ActivateItem(AActor* Activator) = 0;
    // �� �������� ����(Ÿ��)�� ��ȯ (��: "Coin", "Mine" ��)
    virtual FName GetItemType() const = 0;
};
