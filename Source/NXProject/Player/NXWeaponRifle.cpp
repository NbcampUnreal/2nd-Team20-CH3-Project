#include "Player/NXWeaponRifle.h"
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/DamageType.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

ANXWeaponRifle::ANXWeaponRifle()
{
	PrimaryActorTick.bCanEverTick = false;

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

void ANXWeaponRifle::Fire()
{
    if (!CanFire()) return;

   
    UpdateAmmo(CurrentAmmo - 1);

   
    UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
    if (IsValid(AnimInstance) && IsValid(FireMontage))
    {
        AnimInstance->Montage_Play(FireMontage);
    }

    
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PlayerController) return;

    FVector Start = MuzzleOffset->GetComponentLocation();
    FVector ForwardVector = PlayerController->GetControlRotation().Vector();
    FVector End = Start + (ForwardVector * 5000.0f);

    //FVector Start = Mesh->GetSocketLocation(TEXT("MuzzleOffset")); 
    //FVector RightDirection = Mesh->GetRightVector();; 
    //FVector End = Start + (RightDirection * 5000.0f); 

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
    {
       
        ANXNonPlayerCharacter* NXNonPlayerCharacter = Cast<ANXNonPlayerCharacter>(HitResult.GetActor());
        if (NXNonPlayerCharacter)
        {
            FPointDamageEvent DamageEvent(10.0f, HitResult, ForwardVector, nullptr); 
            NXNonPlayerCharacter->TakeDamage(10.0f, DamageEvent, PlayerController, this);
        }

        
        DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 1.0f, 0, 2.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 2.0f);
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
    GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ANXWeaponRifle::FinishReload, ReloadTime, false);
}

void ANXWeaponRifle::FinishReload()
{
    CurrentAmmo = MaxAmmo;
    bIsReloading = false;
}

void ANXWeaponRifle::ResetFire()
{
    bCanFire = true;
}

void ANXWeaponRifle::UpdateAmmo(int32 NewAmmo)
{
    CurrentAmmo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
    OnAmmoChanged.Broadcast(CurrentAmmo); 

    UE_LOG(LogTemp, Log, TEXT("현재 탄약: %d"), CurrentAmmo);
}
void ANXWeaponRifle::StartFiring()
{
    if (!bIsFiring)
    {
        bIsFiring = true;
        Fire();  
        GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ANXWeaponRifle::ContinuousFire, FireRate, true);
    }
}

void ANXWeaponRifle::StopFiring()
{
    bIsFiring = false;
    GetWorld()->GetTimerManager().ClearTimer(FireRateTimer); 
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