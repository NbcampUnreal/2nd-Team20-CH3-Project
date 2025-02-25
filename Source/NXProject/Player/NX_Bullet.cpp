#include "Player/NX_Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h" 
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"

 


ANX_Bullet::ANX_Bullet()
{

	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponet"));
	RootComponent = StaticMeshComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;

}


void ANX_Bullet::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ANX_Bullet::OnHit);
}


void ANX_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANX_Bullet::OnHit(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OntherComponent, 
	FVector Normallmpulse, 
	const FHitResult& Hit)
{
	if (!IsValid(OtherActor))
	{
		return; // **충돌한 오브젝트가 유효하지 않다면 리턴**
	}

	// **충돌한 대상이 AI인지 확인**
	ANXNonPlayerCharacter* HitAI = Cast<ANXNonPlayerCharacter>(OtherActor);
	if (!IsValid(HitAI))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총알 충돌] 대상이 AI가 아님, 데미지 적용 안 함"));
		return; // **플레이어나 환경 오브젝트라면 리턴**
	}

	// **총알을 발사한 캐릭터 확인**
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	// **총알을 발사한 캐릭터가 플레이어인지 확인**
	ANXPlayerCharacter* Shooter = Cast<ANXPlayerCharacter>(OwnerActor->GetOwner());
	if (!IsValid(Shooter))
	{
		UE_LOG(LogTemp, Warning, TEXT("[총알 발사자 오류] 발사한 캐릭터가 플레이어가 아님"));
		return; // **플레이어가 아니라면 리턴**
	}

	// **데미지 적용 (Getter 함수 사용)**
	float DamageAmount = Shooter->GetAttackDamage();

	// **데미지 적용 후 로그 출력**
	FDamageEvent DamageEvent;
	HitAI->TakeDamage(DamageAmount, DamageEvent, Shooter->GetController(), this);
	UE_LOG(LogTemp, Warning, TEXT("[총알 충돌] 플레이어 %s가 AI %s에게 %f의 데미지를 입힘"),
		*Shooter->GetName(), *HitAI->GetName(), DamageAmount);

	// **총알 제거 (Destroy)**
	Destroy();
}



