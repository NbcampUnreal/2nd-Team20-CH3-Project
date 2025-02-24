#include "Player/NXWeaponRifle.h"
#include "Player/NXPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/NX_Bullet.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"

ANXWeaponRifle::ANXWeaponRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);
}

void ANXWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
}

void ANXWeaponRifle::Fire()
{
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (IsValid(AnimInstance) && IsValid(FireMontage))
	{
		AnimInstance->Montage_Play(FireMontage);
	}

	if (!IsValid(Bullet))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총기 오류] 총알 클래스가 설정되지 않음"));
		return;
	}

	AActor* WeaponOwner = GetOwner();
	if (!IsValid(WeaponOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총기 오류] 총의 소유자가 없음"));
		return;
	}

	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(WeaponOwner);
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총기 제한] 플레이어가 아닌 캐릭터는 총을 사용할 수 없음"));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Player->GetController());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총기 오류] 플레이어 컨트롤러를 찾을 수 없음"));
		return;
	}

	FVector SpawnLocation = MuzzleOffset->GetComponentLocation();
	FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor<ANX_Bullet>(Bullet, SpawnLocation, SpawnRotation, SpawnParams);

	UE_LOG(LogTemp, Warning, TEXT("[총 발사] 플레이어 %s가 총을 발사함"), *Player->GetName());
}
