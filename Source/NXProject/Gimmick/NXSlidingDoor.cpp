// NXSlidingDoor.cpp
#include "Gimmick/NXSlidingDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"

ANXSlidingDoor::ANXSlidingDoor()
{
    // ƽ �Լ� Ȱ��ȭ
    PrimaryActorTick.bCanEverTick = true;

    // ��Ʈ �� ������Ʈ ����
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootSceneComponent);

    // ��ȣ�ۿ� �ڽ� ������Ʈ ���� �� ����
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootSceneComponent);

    // ��Ȯ�� �浹 ����
    InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBox->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // �浹 �ڽ� ũ�� ���� (�ʿ信 ���� ����)
    InteractionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

    // ������� ���� �ڽ� ǥ�� (�ʿ� �� ����)
    InteractionBox->SetHiddenInGame(false);

    // ���� �޽� ������Ʈ ����
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootSceneComponent);

    // �����̵� ���� �⺻�� ����
    SlideDirection = FVector(1.0f, 0.0f, 0.0f);

    // ������ �̺�Ʈ ���ε�
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapEnd);
}

void ANXSlidingDoor::BeginPlay()
{
    Super::BeginPlay();

    // �ʱ� ��ġ ����
    InitialLocation = DoorMesh->GetRelativeLocation();

    // ��ȣ�ۿ� �ڽ� ũ�� ���� (�������Ʈ���� ������ ��)
    InteractionBox->SetBoxExtent(InteractionBoxExtent);

    // ����� ���� ���
    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Initial Location: %s"), *InitialLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Interaction Box Extent: %s"), *InteractionBoxExtent.ToString());
}

void ANXSlidingDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // �� ������ ������Ʈ
    UpdateDoorMovement(DeltaTime);
}

void ANXSlidingDoor::OnDoorOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    // �÷��̾� ĳ�������� Ȯ��
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        // ���� ������Ʈ
        bPlayerInRange = true;
        bCanOpenDoor = true;

        // �� ����
        OpenDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Entered Door Range"));

        // ����� �޽��� �߰�
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Door Opened by Player"));
        }
    }
}

void ANXSlidingDoor::OnDoorOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    // �÷��̾� ĳ�������� Ȯ�� (�±� üũ�� ����ȭ)
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        // ���� ������Ʈ
        bPlayerInRange = false;

        // �� �ݱ� ����
        CloseDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Left Door Range"));

        // ����� �޽��� �߰�
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player Left Door Range"));
        }
    }
}

void ANXSlidingDoor::OpenDoor()
{
    // �̹� �����ְų� ������ ���� �ƴ� ��쿡�� ����
    if (!bIsDoorOpened && !bIsOpening)
    {
        bIsOpening = true;
        bIsClosing = false;
        CurrentTime = 0.0f;

        // ������: �ִϸ��̼� ���
        if (OpenAnimation)
        {
            // �ִϸ��̼� ��� ���� (�ʿ��� ��� ����)
            UE_LOG(LogTemp, Warning, TEXT("Door Open Animation"));
        }

        UE_LOG(LogTemp, Warning, TEXT("Door Opening"));
    }
}

void ANXSlidingDoor::CloseDoor()
{
    // �̹� �����ְų� ������ ���� �ƴ� ��쿡�� ����
    if (bIsDoorOpened && !bIsClosing)
    {
        bIsClosing = true;
        bIsOpening = false;
        CurrentTime = 0.0f;

        UE_LOG(LogTemp, Warning, TEXT("Door Closing"));
    }
}

void ANXSlidingDoor::UpdateDoorMovement(float DeltaTime)
{
    // ���� ����
    if (bIsOpening)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / SlideDuration, 0.0f, 1.0f);

        // �ε巯�� ����(Lerp) ���
        FVector TargetLocation = InitialLocation + SlideDirection * SlideDistance;
        FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        // ���� �Ϸ�
        if (Alpha >= 1.0f)
        {
            bIsOpening = false;
            bIsDoorOpened = true;
            CurrentTime = 0.0f;

            UE_LOG(LogTemp, Warning, TEXT("Door Fully Opened"));
        }
    }
    // �ݱ� ����
    // �ݱ� ����
    else if (bIsClosing)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / CloseDuration, 0.0f, 1.0f);  // CloseDuration ��� �� �ִ밪 1.0f�� ����

        // �ε巯�� ����(Lerp) ���
        FVector NewLocation = FMath::Lerp(DoorMesh->GetRelativeLocation(), InitialLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        // �ݱ� �Ϸ�
        if (Alpha >= 1.0f)
        {
            bIsClosing = false;
            bIsDoorOpened = false;
            CurrentTime = 0.0f;

            UE_LOG(LogTemp, Warning, TEXT("Door Fully Closed"));
        }
    }
}

