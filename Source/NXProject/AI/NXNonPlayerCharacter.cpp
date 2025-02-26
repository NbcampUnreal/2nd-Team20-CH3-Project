#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/NXAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Delegates/Delegate.h"


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
    : bIsNowAttacking(false)
    , bIsKnockedBack(false)
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

        FVector Start = GetActorLocation();  
        FVector End = Start + GetActorForwardVector() * AttackRange;  

        TArray<FHitResult> HitResults;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); 

       
        bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), CollisionParams);

        if (bHit)
        {
            for (const FHitResult& HitResult : HitResults)
            {
                ANXCharacterBase* TargetCharacter = Cast<ANXCharacterBase>(HitResult.GetActor());
                if (TargetCharacter && TargetCharacter != this) 
                {
                   
                    float DamageAmount = GetAttackDamage();

                    
                    FPointDamageEvent DamageEvent(DamageAmount, HitResult, HitResult.ImpactPoint - Start, nullptr);

                    
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

AActor* ANXNonPlayerCharacter::GetDamageSource() const
{
    return DamageSource;
}

float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    this->DamageSource = DamageCauser;
    this->bIsKnockedBack = true;

    float Health = GetCurrentHealth();

    const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
    FVector HitDirection = FVector::ZeroVector;

    if (PointDamageEvent)
    {
        FName HitBone = PointDamageEvent->HitInfo.BoneName;
        HitDirection = -PointDamageEvent->ShotDirection; // 피격 방향 설정 (공격 방향의 반대)


        if (HitBone == FName("head")) 
        {
            DamageAmount += HeadShotDamage; 
        }
        else if (HitBone == FName("torso")) 
        {
            DamageAmount += BodyShotDamage;
        }
        else if (HitBone == FName("Arm")) 
        {
            DamageAmount += ArmLegDamage;
        }
        else if (HitBone == FName("leg"))
        {
            DamageAmount += ArmLegDamage;
        }
    }
    Health -= DamageAmount;

    Health = FMath::Clamp(Health, 0.0f, GetMaxHealth()); // 0 이하 방지

    SetHealth(Health);

    UE_LOG(LogTemp, Warning, TEXT("[AI 피격] 데미지: %f, 적용 후 남은 체력: %f"), DamageAmount, Health);


    if (Health <= 0.f)
    {
        UE_LOG(LogTemp, Error, TEXT(" AI 사망! 제거됩니다."));
        Die();
    }

    if (DamageCauser)
    {
        FVector KnockBackDir = GetActorLocation() - DamageCauser->GetActorLocation();
        KnockBackDir.Normalize();

        float KnockBackStrength = 10000.f;  
        ApplyKnockBack(KnockBackDir, KnockBackStrength);
    }
    return DamageAmount;
}

void ANXNonPlayerCharacter::ApplyKnockBack(FVector Direction, float Strength)
{
    FVector KnockbackImpulse = Direction * Strength;

    UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent());
    if (RootComp)
    {
        //RootComp->AddImpulse(KnockbackImpulse, NAME_None, true);
        LaunchCharacter(KnockbackImpulse, false, false);
    }

    UNXAnimInstance* AnimInstance = Cast<UNXAnimInstance>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {
        if (bIsNowAttacking)  
        {
            if (AnimInstance->Montage_IsPlaying(AttackMontage))
            {
                AnimInstance->Montage_Stop(0.2f, AttackMontage);
            }
        }

        if (KnockBackMontage)
        {
            AnimInstance->Montage_Play(KnockBackMontage);
            OnKnockBackMontageEndedDelegate.BindUFunction(this, FName("OnKnockBackMontageEnded"));
            AnimInstance->OnMontageEnded.AddDynamic(this, &ANXNonPlayerCharacter::OnKnockBackMontageEnded);
        }
    }

    bIsKnockedBack = true;
}


void ANXNonPlayerCharacter::OnKnockBackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == KnockBackMontage)
    {
        bIsKnockedBack = false;

    }
}

//float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
//	AController* EventInstigator, AActor* DamageCauser)
//{
//	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	
//	float NewHealth = GetCurrentHealth() - ActualDamage;
//	NewHealth = FMath::Clamp(NewHealth, 0.0f, GetMaxHealth());
//
//	
//	UE_LOG(LogTemp, Warning, TEXT("Damage: %f, HP: %f"), ActualDamage, NewHealth);
//
//	if (NewHealth <= 0.0f)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Death!"));
//		Die();
//	}
//
//	return ActualDamage;
//}
