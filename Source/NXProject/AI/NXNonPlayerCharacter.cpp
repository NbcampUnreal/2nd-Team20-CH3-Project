#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/NXAnimInstance.h"
#include "Engine/DamageEvents.h"

ANXNonPlayerCharacter::ANXNonPlayerCharacter()
    : bIsNowAttacking(false)
    , PatrolRadius(200)
    , DetectRadius(100)
    , AttackRange(50)
    , SphereRadius(100.f)
    , HeadShotDamage(100.f)
    , BodyShotDamage(50.f)
    , ArmLegDamage(20.f)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    
    // LineTrace를 감지
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void ANXNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
		
		GetCharacterMovement()->MaxWalkSpeed = GetNormalSpeed();
	}
}

void ANXNonPlayerCharacter::BeginAttack()
{
    UNXAnimInstance* AnimInstance = Cast<UNXAnimInstance>(GetMesh()->GetAnimInstance());
    checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
    {
        AnimInstance->Montage_Play(AttackMontage);

        bIsNowAttacking = true;

        if (OnAttackMontageEndedDelegate.IsBound() == false)
        {
            OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
            AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
        }

        // 공격 범위 내의 적을 찾기 위해 Line Trace 사용 -> Sphere Trace로 변경
        FVector Start = GetActorLocation();  // 현재 캐릭터 위치
        FVector End = Start + GetActorForwardVector() * AttackRange;  // 공격 범위 내 끝 지점

        TArray<FHitResult> HitResults;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);  // 자신을 무시하도록 추가

        // Sphere Trace로 충돌체 찾기
        bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), CollisionParams);

        if (bHit)
        {
            for (const FHitResult& HitResult : HitResults)
            {
                ANXCharacterBase* TargetCharacter = Cast<ANXCharacterBase>(HitResult.GetActor());
                if (TargetCharacter && TargetCharacter != this)  // 자신과 겹친 경우 제외
                {
                    // 공격 데미지
                    float DamageAmount = GetAttackDamage();

                    // 데미지 이벤트 생성
                    FPointDamageEvent DamageEvent(DamageAmount, HitResult, HitResult.ImpactPoint - Start, nullptr);

                    // 대상에게 데미지 전달
                    TargetCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);

                    UE_LOG(LogTemp, Warning, TEXT("AI Attack! Damage: %f"), DamageAmount);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Sphere Trace did not hit anything."));
        }
    }
}

void ANXNonPlayerCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    bIsNowAttacking = false;

    if (OnAttackMontageEndedDelegate.IsBound() == true)
    {
        OnAttackMontageEndedDelegate.Unbind();
    }
}

float ANXNonPlayerCharacter::GetPatrolRadius() const
{
    return PatrolRadius;
}

float ANXNonPlayerCharacter::GetDetectRadius() const
{
    return DetectRadius;
}
float ANXNonPlayerCharacter::GetAttackRange() const
{
    return AttackRange;
}

float ANXNonPlayerCharacter::GetSphereRadius() const
{
    return SphereRadius;
}

float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float Health = GetCurrentHealth();

    const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
    if (PointDamageEvent)
    {
        FName HitBone = PointDamageEvent->HitInfo.BoneName;

        if (HitBone == FName("head")) 
        {
            DamageAmount += HeadShotDamage; 
        }
        else if (HitBone == FName("torso")) 
        {
            DamageAmount += BodyShotDamage;
        }
        else if (HitBone == FName("leg")) 
        {
            DamageAmount += ArmLegDamage;
        }
        else if (HitBone == FName("leg"))
        {
            DamageAmount += ArmLegDamage;
        }
    }
    Health -= DamageAmount;

    if (Health <= 0.f)
    {
        Die();
    }
    return DamageAmount;
}








