#include "Gimmick/NXPortal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ANXPortal::ANXPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 씬 컴포넌트 생성
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootSceneComponent);

	PortalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBox"));
	PortalBox->SetupAttachment(RootSceneComponent);
	PortalBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PortalBox->SetCollisionObjectType(ECC_WorldDynamic);
	PortalBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	PortalBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PortalBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootSceneComponent);

	// 오버랩 이벤트 바인딩
	PortalBox->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnPortalOverlapBegin);
}

void ANXPortal::BeginPlay()
{
	Super::BeginPlay();

	// 여기에 필요한 초기화 코드를 추가할 수 있습니다
}

void ANXPortal::OnPortalOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 플레이어 캐릭터인지 확인
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		// 설정된 레벨로 이동
		UGameplayStatics::OpenLevel(this, TargetLevelName);
	}
}


