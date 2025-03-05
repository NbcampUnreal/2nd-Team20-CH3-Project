#include "Gimmick/NXPortal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ANXPortal::ANXPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	// ��Ʈ �� ������Ʈ ����
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

	// ������ �̺�Ʈ ���ε�
	PortalBox->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnPortalOverlapBegin);
}

void ANXPortal::BeginPlay()
{
	Super::BeginPlay();

	// ���⿡ �ʿ��� �ʱ�ȭ �ڵ带 �߰��� �� �ֽ��ϴ�
}

void ANXPortal::OnPortalOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// �÷��̾� ĳ�������� Ȯ��
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		// ������ ������ �̵�
		UGameplayStatics::OpenLevel(this, TargetLevelName);
	}
}


