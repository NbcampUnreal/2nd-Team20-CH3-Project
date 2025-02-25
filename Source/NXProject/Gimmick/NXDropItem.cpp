#include "Gimmick/NXDropItem.h"

ANXDropItem::ANXDropItem()
{
	// ���� �⺻���� 0���� ����
	PointValue = 0;
	ItemType = "DefaultDropItem";
}

void ANXDropItem::ActivateItem(AActor* Activator)
{
    // �÷��̾� �±� Ȯ��
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // ���� ȹ�� ����� �޽���
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));

        // �θ� Ŭ������ ���ǵ� ������ �ı� �Լ� ȣ��
        DestroyItem();
    }
}
