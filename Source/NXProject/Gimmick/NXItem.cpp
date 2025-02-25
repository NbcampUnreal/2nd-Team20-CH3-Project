#include "Gimmick/NXItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

ANXItem::ANXItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // ��Ʈ ������Ʈ ���� �� ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // �浹 ������Ʈ ���� �� ����
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // ��ħ�� �����ϴ� �������� ����
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // ��Ʈ ������Ʈ�� ����
    Collision->SetupAttachment(Scene);

    // ����ƽ �޽� ������Ʈ ���� �� ����
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
    // �޽ð� ���ʿ��ϰ� �浹�� ���� �ʵ��� �ϱ� ����, ������ NoCollision ������ ������ �� ����.

// Overlap �̺�Ʈ ���ε�
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ANXItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ANXItem::OnItemEndOverlap);
}

// �÷��̾ ������ ������ ������ �� ����
void ANXItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)

{
    // OtherActor�� �÷��̾����� Ȯ�� ("Player" �±� Ȱ��)
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
        // ������ ��� (ȹ��) ���� ȣ��
        ActivateItem(OtherActor);
    }
}


// �÷��̾ ������ ������ ����� �� ����
void ANXItem::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
	// �⺻�� �� �Լ� - �ʿ��ϴٸ� �ڽ� Ŭ�������� Ȱ��
}

// �������� ���(Activate)�Ǿ��� �� ����
void ANXItem::ActivateItem(AActor* Activator)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}

// ������ ������ ��ȯ
FName ANXItem::GetItemType() const
{
	return ItemType;
}

// �������� �ı�(����)�ϴ� �Լ�
void ANXItem::DestroyItem()
{
	// AActor���� �����ϴ� Destroy() �Լ��� ��ü ����
	Destroy();
}

