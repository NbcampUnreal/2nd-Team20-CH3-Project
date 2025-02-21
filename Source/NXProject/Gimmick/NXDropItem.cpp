#include "Gimmick/NXDropItem.h"

ANXDropItem::ANXDropItem()
{
	// 점수 기본값을 0으로 설정
	PointValue = 0;
	ItemType = "DefaultDropItem";
}

void ANXDropItem::ActivateItem(AActor* Activator)
{
    // 플레이어 태그 확인
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // 점수 획득 디버그 메시지
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));

        // 부모 클래스에 정의된 아이템 파괴 함수 호출
        DestroyItem();
    }
}
