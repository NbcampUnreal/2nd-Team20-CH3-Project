#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXWeaponRifle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, CurrentAmmo);

UCLASS()
class NXPROJECT_API ANXWeaponRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ANXWeaponRifle();


protected:

	virtual void BeginPlay() override;
	
public:


	void Fire();
	void StartFiring();
	void StopFiring();
	void Reload();
	bool CanFire() const;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

private:

	void FinishReload();
	void ResetFire();
	void UpdateAmmo(int32 NewAmmo);
	void ContinuousFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	bool bIsReloading;
	bool bCanFire;
	bool bIsFiring;

	FTimerHandle FireRateTimer;
	FTimerHandle ReloadTimer;
};
