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


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ANXWeaponRifle> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<ANXWeaponRifle> WeaponInstance;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    bool GetIsCrouching() const;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void StartJump(const FInputActionValue& value);

	UFUNCTION()
	void StopJump(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	UFUNCTION()
	void StartAttack(const FInputActionValue& value);

	UFUNCTION()
	void StopAttack(const FInputActionValue& value);

	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);

	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);

	UFUNCTION()
	void StartReload(const FInputActionValue& value);

	UFUNCTION()
	void StopReload(const FInputActionValue& value);

	UFUNCTION()
	void InputQuickSlot01(const FInputActionValue& InValue);

	UFUNCTION()
	void InputQuickSlot02(const FInputActionValue& InValue);

	
};
