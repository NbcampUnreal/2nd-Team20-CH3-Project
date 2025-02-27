// NXSlidingDoor.cpp
#include "Gimmick/NXSlidingDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"

ANXSlidingDoor::ANXSlidingDoor()
{
    // 틱 함수 활성화
    PrimaryActorTick.bCanEverTick = true;

    // 루트 씬 컴포넌트 생성
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootSceneComponent);

    // 상호작용 박스 컴포넌트 생성 및 설정
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootSceneComponent);

    // 정확한 충돌 설정
    InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBox->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // 충돌 박스 크기 설정 (필요에 따라 조정)
    InteractionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

    // 디버깅을 위해 박스 표시 (필요 시 제거)
    InteractionBox->SetHiddenInGame(false);

    // 도어 메시 컴포넌트 생성
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootSceneComponent);

    // 슬라이드 방향 기본값 설정
    SlideDirection = FVector(1.0f, 0.0f, 0.0f);

    // 오버랩 이벤트 바인딩
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ANXSlidingDoor::OnDoorOverlapEnd);
}

void ANXSlidingDoor::BeginPlay()
{
    Super::BeginPlay();

    // 초기 위치 저장
    InitialLocation = DoorMesh->GetRelativeLocation();

    // 상호작용 박스 크기 적용 (블루프린트에서 설정한 값)
    InteractionBox->SetBoxExtent(InteractionBoxExtent);

    // 디버그 정보 출력
    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Initial Location: %s"), *InitialLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Door BeginPlay - Interaction Box Extent: %s"), *InteractionBoxExtent.ToString());
}

void ANXSlidingDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 문 움직임 업데이트
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
    // 플레이어 캐릭터인지 확인
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        // 상태 업데이트
        bPlayerInRange = true;
        bCanOpenDoor = true;

        // 문 열기
        OpenDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Entered Door Range"));

        // 디버그 메시지 추가
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
    // 플레이어 캐릭터인지 확인 (태그 체크를 간소화)
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character && (OtherActor->ActorHasTag(FName("Player")) || Character->IsPlayerControlled()))
    {
        // 상태 업데이트
        bPlayerInRange = false;

        // 문 닫기 시작
        CloseDoor();

        UE_LOG(LogTemp, Warning, TEXT("Player Left Door Range"));

        // 디버그 메시지 추가
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player Left Door Range"));
        }
    }
}

void ANXSlidingDoor::OpenDoor()
{
    // 이미 열려있거나 열리는 중이 아닌 경우에만 실행
    if (!bIsDoorOpened && !bIsOpening)
    {
        bIsOpening = true;
        bIsClosing = false;
        CurrentTime = 0.0f;

        // 선택적: 애니메이션 재생
        if (OpenAnimation)
        {
            // 애니메이션 재생 로직 (필요한 경우 구현)
            UE_LOG(LogTemp, Warning, TEXT("Door Open Animation"));
        }

        UE_LOG(LogTemp, Warning, TEXT("Door Opening"));
    }
}

void ANXSlidingDoor::CloseDoor()
{
    // 이미 닫혀있거나 닫히는 중이 아닌 경우에만 실행
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
    // 열기 상태
    if (bIsOpening)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / SlideDuration, 0.0f, 1.0f);

        // 부드러운 보간(Lerp) 사용
        FVector TargetLocation = InitialLocation + SlideDirection * SlideDistance;
        FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        // 열기 완료
        if (Alpha >= 1.0f)
        {
            bIsOpening = false;
            bIsDoorOpened = true;
            CurrentTime = 0.0f;

            UE_LOG(LogTemp, Warning, TEXT("Door Fully Opened"));
        }
    }
    // 닫기 상태
    // 닫기 상태
    else if (bIsClosing)
    {
        CurrentTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentTime / CloseDuration, 0.0f, 1.0f);  // CloseDuration 사용 및 최대값 1.0f로 수정

        // 부드러운 보간(Lerp) 사용
        FVector NewLocation = FMath::Lerp(DoorMesh->GetRelativeLocation(), InitialLocation, Alpha);
        DoorMesh->SetRelativeLocation(NewLocation);

        // 닫기 완료
        if (Alpha >= 1.0f)
        {
            bIsClosing = false;
            bIsDoorOpened = false;
            CurrentTime = 0.0f;

            UE_LOG(LogTemp, Warning, TEXT("Door Fully Closed"));
        }
    }
}

