#include "Gimmick/NXDropItem.h"

ANXDropItem::ANXDropItem()
{
	PointValue = 10;
	ItemType = "DropItem";
}

void ANXDropItem::ActivateItem(AActor* Activator)
{
	DestroyItem(); // 아이템 소멸(부모의 DestroyItem() 혹은 자체 로직)
}
