#include "Player/NX_Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h" 
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Player/NXCharacterBase.h"
 


ANX_Bullet::ANX_Bullet()
{

	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponet"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(TEXT("Projectile")); // collision

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	InitialLifeSpan = 5.0f; // Bullet LifeSpan time
}


void ANX_Bullet::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 감지를 위한 바인딩 (오류 방지)
	if (IsValid(StaticMeshComponent))
	{
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ANX_Bullet::OnHit);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is NULL! Delegate binding failed."));
	}

	// **총알이 안 보이는 문제 해결**
	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);

	// **총알의 이동 속도 강제 설정**
	if (IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * 3000.0f;
	}
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
	UE_LOG(LogTemp, Warning, TEXT("총알 충돌 감지됨! 대상: %s"), *OtherActor->GetName());

	if (!IsValid(OtherActor) || OtherActor == GetOwner())
	{
		return; // **자기 자신을 맞추면 무시**
	}

	ANXNonPlayerCharacter* HitAI = Cast<ANXNonPlayerCharacter>(OtherActor);
	if (HitAI)
	{
		float DamageAmount = 20.0f; // **기본 총알 데미지**

		
		FPointDamageEvent DamageEvent(DamageAmount, Hit, Normallmpulse, nullptr);

		
		float AppliedDamage = HitAI->TakeDamage(DamageAmount, DamageEvent, GetOwner()->GetInstigatorController(), this);

		UE_LOG(LogTemp, Warning, TEXT("[총알 충돌] %s가 AI %s에게 %f의 데미지를 입힘 | 남은 체력: %f"),
			*GetOwner()->GetName(), *HitAI->GetName(), DamageAmount, HitAI->GetCurrentHealth());

		
		if (HitAI->GetCurrentHealth() <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AI 사망] AI가 사망했습니다!"));
			HitAI->Destroy();
		}
	}

	Destroy(); // **총알 삭제**
}


//{
//	if (!IsValid(OtherActor))
//	{
//		return; // **충돌한 오브젝트가 유효하지 않다면 리턴**
//	}
//
//	// **충돌한 대상이 AI인지 확인**
//	ANXNonPlayerCharacter* HitAI = Cast<ANXNonPlayerCharacter>(OtherActor);
//	if (!IsValid(HitAI))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("[총알 충돌] 대상이 AI가 아님, 데미지 적용 안 함"));
//		return; // **플레이어나 환경 오브젝트라면 리턴**
//	}
//
//	// **총알을 발사한 캐릭터 확인**
//	AActor* OwnerActor = GetOwner();
//	if (!IsValid(OwnerActor))
//	{
//		return;
//	}
//
//	// **총알을 발사한 캐릭터가 플레이어인지 확인**
//	ANXPlayerCharacter* Shooter = Cast<ANXPlayerCharacter>(OwnerActor->GetOwner());
//	if (!IsValid(Shooter))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("[총알 발사자 오류] 발사한 캐릭터가 플레이어가 아님"));
//		return; // **플레이어가 아니라면 리턴**
//	}
//
//	// **데미지 적용 (Getter 함수 사용)**
//	float DamageAmount = Shooter->GetAttackDamage();
//
//	// **데미지 적용 후 로그 출력**
//	FDamageEvent DamageEvent;
//	HitAI->TakeDamage(DamageAmount, DamageEvent, Shooter->GetController(), this);
//	UE_LOG(LogTemp, Warning, TEXT("[총알 충돌] 플레이어 %s가 AI %s에게 %f의 데미지를 입힘"),
//		*Shooter->GetName(), *HitAI->GetName(), DamageAmount);
//
//	// **총알 제거 (Destroy)**
//	Destroy();
//}



