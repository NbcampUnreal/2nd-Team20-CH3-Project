#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/NXAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Delegates/Delegate.h"
#include "Components/ProgressBar.h" 
#include "Components/CapsuleComponent.h"
#include "Player/NXPlayerCharacter.h"
#include "Player/NXWeaponRifle.h"
#include "Kismet/GameplayStatics.h"       
#include "Particles/ParticleSystem.h"             


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
    : bIsNowAttacking(false)
    , bIsKnockedBack(false)
    , bIsInAttackRange(false)
    , PatrolRadius(200)
    , DetectRadius(100)
    , AttackRange(50)
    , SphereRadius(100.f)
    , HeadShotDamage(100.f)
    , BodyShotDamage(50.f)
    , ArmLegDamage(20.f)
    , KnockBackStrength(1000.f)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
    HealthBarComponent->SetupAttachment(RootComponent);

    HealthBarComponent->SetWidgetSpace(EWidgetSpace::World);

    HealthBarComponent->SetDrawSize(FVector2D(200, 20));

    HealthBarComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
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
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	}

    if (HealthBarComponent && HealthBarClass)
    {
        UUserWidget* HealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), HealthBarClass);
        if (HealthBarWidget)
        {
            HealthBarComponent->SetWidget(HealthBarWidget);
        }
    }
}
//void ANXNonPlayerCharacter::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    if (bIsNowAttacking)
//    {
//        if (ANXAIController* AIController = Cast<ANXAIController>(GetController()))
//        {
//            AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsInAttackRange"), true);
//        }
//    }
//}



void ANXNonPlayerCharacter::UpdateHealthBar()
{
    if (HealthBarComponent)
    {
        UUserWidget* Widget = Cast<UUserWidget>(HealthBarComponent->GetWidget());
        if (Widget)
        {
            UProgressBar* HealthBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HealthBar")));
            if (HealthBar)
            {
                float HealthPercent = GetCurrentHealth() / GetMaxHealth();
                HealthBar->SetPercent(HealthPercent);
            }
        }
    }
}

void ANXNonPlayerCharacter::OnCheckHit()
{
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

void ANXNonPlayerCharacter::BeginAttack()
{
    UNXAnimInstance* AnimInstance = Cast<UNXAnimInstance>(GetMesh()->GetAnimInstance());
    checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));


    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    GetCharacterMovement()->bOrientRotationToMovement = true;


    if (IsValid(AnimInstance))
    {
        AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
    }

    if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
    {
        AnimInstance->Montage_Play(AttackMontage, 1.0f, EMontagePlayReturnType::Duration, 0.1f);

        bIsNowAttacking = true;

        if (OnAttackMontageEndedDelegate.IsBound() == false)
        {
            OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
            AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
        }

        if (ANXAIController* AIController = Cast<ANXAIController>(GetController()))
        {
            if (ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
            {
                AIController->MoveToActor(Player, GetAttackRange());

                UE_LOG(LogTemp, Warning, TEXT("AI is attacking and moving towards player!"));
            }
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

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
    if (bIsDying)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] 이미 사망 처리 중, 데미지 무시"));
        return 0.f;
    }

    if (!DamageCauser)
    {
        return 0.f;
    }

    bool bIsMeleeAttack = DamageCauser->IsA(ANXPlayerCharacter::StaticClass());

    AActor* WeaponOwner = DamageCauser->GetOwner();
    bool bIsGunAttack = (WeaponOwner && WeaponOwner->IsA(ANXPlayerCharacter::StaticClass()));

    if (!bIsMeleeAttack && !bIsGunAttack)
    {
        return 0.f;
    }

    this->DamageSource = DamageCauser;
    this->bIsKnockedBack = true;

    float Health = GetCurrentHealth();

    const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
    FVector HitDirection = FVector::ZeroVector;

    if (PointDamageEvent)
    {
        FName HitBone = PointDamageEvent->HitInfo.BoneName;
        HitDirection = -PointDamageEvent->ShotDirection;

        if (HitBone == FName("head") || HitBone == FName("torso") || HitBone == FName("Arm") || HitBone == FName("leg"))
        {
            if (DamageCauser->GetWorld())
            {
                FVector SpawnLocation = PointDamageEvent->HitInfo.ImpactPoint;
                UParticleSystem* BloodEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/BSP_FPS_TPS_Particles/Particles/BulletImpact/PS_Bullet_Impact_Flesh_1.PS_Bullet_Impact_Flesh_1")); 

                if (BloodEffect)
                {
                    UGameplayStatics::SpawnEmitterAtLocation(DamageCauser->GetWorld(), BloodEffect, SpawnLocation, FRotator::ZeroRotator, true);
                }
            }
        }

        if (HitBone == FName("head"))
        {
            DamageAmount += HeadShotDamage;
        }
        else if (HitBone == FName("torso"))
        {
            DamageAmount += BodyShotDamage;
        }
        else if (HitBone == FName("Arm") || HitBone == FName("leg"))
        {
            DamageAmount += ArmLegDamage;
        }
    }

    Health -= DamageAmount;
    Health = FMath::Clamp(Health, 0.0f, GetMaxHealth());
    UE_LOG(LogTemp, Warning, TEXT("[AI 피격] Clamp 이후 체력: %f"), Health);

    SetHealth(Health);

    UpdateHealthBar();

    UE_LOG(LogTemp, Warning, TEXT("[AI 피격] 데미지: %f, 적용 후 남은 체력: %f"), DamageAmount, Health);

    if (Health <= 0.f)
    {
        UE_LOG(LogTemp, Error, TEXT("[TakeDamage] Health가 0 이하, Die() 호출!"));

        UE_LOG(LogTemp, Error, TEXT("AI 사망! 제거됩니다."));
        Die();
    }

    if (DamageCauser)
    {
        FVector KnockBackDir = GetActorLocation() - DamageCauser->GetActorLocation();
        KnockBackDir.Normalize();

        ApplyKnockBack(KnockBackDir, KnockBackStrength);
    }
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}



void ANXNonPlayerCharacter::ApplyKnockBack(FVector Direction, float Strength)
{
    FVector KnockbackImpulse = Direction * Strength;

    GetCharacterMovement()->Velocity = FVector::ZeroVector;
    
    const float MaxKnockBackStrength = 800.f;
    KnockbackImpulse = KnockbackImpulse.GetClampedToMaxSize(MaxKnockBackStrength);

    LaunchCharacter(KnockbackImpulse, true, false); 

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

        if (AnimInstance->Montage_IsPlaying(KnockBackMontage))
        {
            AnimInstance->Montage_Stop(0.1f, KnockBackMontage);
        }

        if (GetCharacterMovement()->IsMovingOnGround())
        {
            if (AnimInstance && KnockBackMontage)
            {
                AnimInstance->Montage_Play(KnockBackMontage);

                AnimInstance->OnMontageEnded.RemoveDynamic(this, &ANXNonPlayerCharacter::OnKnockBackMontageEnded);
                AnimInstance->OnMontageEnded.AddDynamic(this, &ANXNonPlayerCharacter::OnKnockBackMontageEnded);
            }
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

