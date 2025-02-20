#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NXItemInterface.generated.h"

// 인터페이스를 UObject 시스템에서 사용하기 위한 기본 매크로
UINTERFACE(MinimalAPI)
class UNXItemInterface : public UInterface
{
    GENERATED_BODY()
};

// 실제 C++ 레벨에서 사용할 함수 원형(시그니처)를 정의
class NXPROJECT_API INXItemInterface
{
    GENERATED_BODY()

public:
    // 플레이어가 이 아이템의 범위에 들어왔을 때 호출
    virtual void OnItemOverlap(AActor* OverlapActor) = 0;
    // 플레이어가 이 아이템의 범위를 벗어났을 때 호출
    virtual void OnItemEndOverlap(AActor* OverlapActor) = 0;
    // 아이템이 사용되었을 때 호출
    virtual void ActivateItem(AActor* Activator) = 0;
    // 이 아이템의 유형(타입)을 반환 (예: "Coin", "Mine" 등)
    virtual FName GetItemType() const = 0;
};
