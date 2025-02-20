#include "Gimmick/NXItem.h"

ANXItem::ANXItem()
{
	// 틱(매 프레임 실행)은 필요 없으므로 끕니다.
	PrimaryActorTick.bCanEverTick = false;
}

// 플레이어가 아이템 범위에 들어왔을 때 동작
void ANXItem::OnItemOverlap(AActor* OverlapActor)
{
	// 기본은 빈 함수 - 각 자식 클래스에서 구현
}

// 플레이어가 아이템 범위를 벗어났을 때 동작
void ANXItem::OnItemEndOverlap(AActor* OverlapActor)
{
	// 기본은 빈 함수 - 필요하다면 자식 클래스에서 활용
}

// 아이템이 사용(Activate)되었을 때 동작
void ANXItem::ActivateItem(AActor* Activator)
{
	// 기본은 빈 함수 - 자식 클래스에서 구현
}

// 아이템 유형을 반환
FName ANXItem::GetItemType() const
{
	return ItemType;
}

// 아이템을 파괴(제거)하는 함수
void ANXItem::DestroyItem()
{
	// AActor에서 제공하는 Destroy() 함수로 객체 제거
	Destroy();
}

