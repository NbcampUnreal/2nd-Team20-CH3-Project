// NXSlidingDoor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "NXSlidingDoor.generated.h"

UCLASS()
class NXPROJECT_API ANXSlidingDoor : public AActor
{
    GENERATED_BODY()
public:
    ANXSlidingDoor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 컴포넌트 선언
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class UBoxComponent* InteractionBox;

    // 문 설정 관련 변수들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float SlideDistance = 200.0f;  // 문이 열리는 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float SlideDuration = 1.0f;  // 문 열리는 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float CloseDuration = 3.0f; // 문 닫히는 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    FVector SlideDirection = FVector(1.0f, 0.0f, 0.0f);  // 문이 열리는 방향

    //추가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float DoorStayOpenDuration = 3.0f;// 문이 열린 상태로 유지되는 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    FVector InteractionBoxExtent = FVector(100.f, 100.f, 100.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    class UAnimSequence* OpenAnimation;  // 선택적 문 열기 애니메이션

private:
    // 상태 변수들
    bool bPlayerInRange = false;
    bool bCanOpenDoor = false;
    bool bIsDoorOpened = false;
    bool bIsOpening = false;
    bool bIsClosing = false;

    // 위치 관련 변수
    FVector InitialLocation;
    float CurrentTime = 0.0f;

    // 추가
    float OpenedTime = 0.0f;  // 문이 열린 시간을 추적
    bool bIsWaitingToClose = false;  // 문 닫기 대기 상태

    // 충돌 이벤트 함수
    UFUNCTION()
    void OnDoorOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnDoorOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    // 문 열기 함수
    void OpenDoor();

    // 문 닫기 함수
    void CloseDoor();

    // 문 움직임 업데이트 함수
    void UpdateDoorMovement(float DeltaTime);
};