#include "Player/NXWeaponRifle.h"
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/DamageType.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"

ANXWeaponRifle::ANXWeaponRifle()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);

	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	ReloadTime = 2.0f;
	FireRate = 0.1f;
	bIsReloading = false;
	bCanFire = true;
    bIsFiring = false;
}

void ANXWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
    UpdateAmmo(CurrentAmmo);
}

int32 ANXWeaponRifle::GetMaxAmmo() const
{
    return MaxAmmo;
}

int32 ANXWeaponRifle::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

void ANXWeaponRifle::Fire()
{
    if (!CanFire()) return;

    UE_LOG(LogTemp, Log, TEXT("Fire() 호출됨 - 현재 탄약: %d, bCanFire: %d, bIsFiring: %d"), CurrentAmmo, bCanFire, bIsFiring);
    UpdateAmmo(CurrentAmmo - 1);

    UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
    if (IsValid(AnimInstance) && IsValid(FireMontage))
    {
        AnimInstance->Montage_Play(FireMontage);
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PlayerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 5000.0f);
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    FVector TargetPoint = TraceEnd;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams))
    {
        TargetPoint = HitResult.ImpactPoint;
    }

    FVector MuzzleLocation = MuzzleOffset->GetComponentLocation();
    FVector Direction = (TargetPoint - MuzzleLocation).GetSafeNormal();

    DrawDebugLine(GetWorld(), MuzzleLocation, TargetPoint, FColor::Red, false, 0.2f, 0, 2.0f);

    if (HitResult.GetActor())
    {
        ANXNonPlayerCharacter* Enemy = Cast<ANXNonPlayerCharacter>(HitResult.GetActor());
        if (Enemy)
        {
            FPointDamageEvent DamageEvent(10.0f, HitResult, Direction, UDamageType::StaticClass());
            Enemy->TakeDamage(10.0f, DamageEvent, PlayerController, this);
        }
    }

    bCanFire = false;
    GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ANXWeaponRifle::ResetFire, FireRate, false);

    if (bIsFiring)
    {
        GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ANXWeaponRifle::Fire, FireRate, true);
    }
}

bool ANXWeaponRifle::CanFire() const
{
    return CurrentAmmo > 0 && !bIsReloading && bCanFire;
}

void ANXWeaponRifle::Reload()
{
    if (bIsReloading || CurrentAmmo == MaxAmmo) return;

    bIsReloading = true;
    bIsFiring = false;
    GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
    GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ANXWeaponRifle::FinishReload, ReloadTime, false);
}

void ANXWeaponRifle::FinishReload()
{
    CurrentAmmo = MaxAmmo;
    bIsReloading = false;
    UpdateAmmo(CurrentAmmo);
}

void ANXWeaponRifle::ResetFire()
{
    bCanFire = true;

    UE_LOG(LogTemp, Log, TEXT("ResetFire() - bCanFire: %d, bIsFiring: %d"), bCanFire, bIsFiring);

    if (bIsFiring) 
    {
        Fire();
    }
}

void ANXWeaponRifle::UpdateAmmo(int32 NewAmmo)
{
    CurrentAmmo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
    OnAmmoChanged.Broadcast(CurrentAmmo); 

    UE_LOG(LogTemp, Log, TEXT("현재 탄약: %d"), CurrentAmmo);
}

void ANXWeaponRifle::AddAmmo(int32 AmmoToAdd)
{
    UpdateAmmo(FMath::Min(CurrentAmmo + AmmoToAdd, MaxAmmo));
}

void ANXWeaponRifle::StartFiring()
{
    if (!bIsFiring && CanFire()) 
    {
        bIsFiring = true;
        UE_LOG(LogTemp, Log, TEXT("StartFiring() - 연속 사격 시작"));
        Fire();  
    }
}

void ANXWeaponRifle::StopFiring()
{
    bIsFiring = false;
    UE_LOG(LogTemp, Log, TEXT("StopFiring() - 연속 사격 중단"));
}

void ANXWeaponRifle::ContinuousFire()
{
    if (bIsFiring && CanFire())
    {
        Fire();  
    }
    else
    {
        StopFiring();  
    }
}