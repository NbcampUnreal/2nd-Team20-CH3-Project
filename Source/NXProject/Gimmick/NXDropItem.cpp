#include "Gimmick/NXDropItem.h"

ANXDropItem::ANXDropItem()
{
	PointValue = 10;
	ItemType = "DropItem";
}

void ANXDropItem::ActivateItem(AActor* Activator)
{
	DestroyItem(); // ������ �Ҹ�(�θ��� DestroyItem() Ȥ�� ��ü ����)
}
