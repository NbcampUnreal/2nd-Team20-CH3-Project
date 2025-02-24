#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "NXPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ANXWeaponRifle;
struct FInputActionValue;

UCLASS()
class NXPROJECT_API ANXPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

public:
	ANXPlayerCharacter();
	virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float RangedAttackRange = 1000.0f; 

	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeAttackRange = 50.0f; 

	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeDamage = 30.0f; 



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ANXWeaponRifle> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<ANXWeaponRifle> WeaponInstance;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<ANXWeaponRifle> WeaponActor;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ANXWeaponRifle> Weapon;

	UFUNCTION(Blueprintpure, Category = "Movement")
	bool GetIsCrouching() const;

	void Move(const FInputActionValue& value);
	void StartJump(const FInputActionValue& value);
	void StopJump(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);
	void StartAttack(); // 원거리
	void StopAttack(const FInputActionValue& value);
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);
	void Reload(const FInputActionValue& value);
	void InputQuickSlot01(const FInputActionValue& InValue);
	void InputQuickSlot02(const FInputActionValue& InValue);
	void Fire(const FInputActionValue& value);
	void StartPunchAttack(); // 근접
	void StopPunchAttack(const FInputActionValue& value);


};