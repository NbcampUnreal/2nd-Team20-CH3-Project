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

    // ������Ʈ ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
    class UBoxComponent* InteractionBox;

    // �� ���� ���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float SlideDistance = 200.0f;  // ���� ������ �Ÿ�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float SlideDuration = 1.0f;  // �� ������ �ð�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float CloseDuration = 3.0f; // �� ������ �ð�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    FVector SlideDirection = FVector(1.0f, 0.0f, 0.0f);  // ���� ������ ����

    //�߰�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    float DoorStayOpenDuration = 3.0f;// ���� ���� ���·� �����Ǵ� �ð�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    FVector InteractionBoxExtent = FVector(100.f, 100.f, 100.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings")
    class UAnimSequence* OpenAnimation;  // ������ �� ���� �ִϸ��̼�

private:
    // ���� ������
    bool bPlayerInRange = false;
    bool bCanOpenDoor = false;
    bool bIsDoorOpened = false;
    bool bIsOpening = false;
    bool bIsClosing = false;

    // ��ġ ���� ����
    FVector InitialLocation;
    float CurrentTime = 0.0f;

    // �߰�
    float OpenedTime = 0.0f;  // ���� ���� �ð��� ����
    bool bIsWaitingToClose = false;  // �� �ݱ� ��� ����

    // �浹 �̺�Ʈ �Լ�
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

    // �� ���� �Լ�
    void OpenDoor();

    // �� �ݱ� �Լ�
    void CloseDoor();

    // �� ������ ������Ʈ �Լ�
    void UpdateDoorMovement(float DeltaTime);
};