#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "NXPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
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

	
};
