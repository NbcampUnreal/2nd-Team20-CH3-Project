#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/NXPlayerController.h" 
#include "Blueprint/UserWidget.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h" 
#include "NXCharacterBase.generated.h"


class ANXPlayerChahracter;

UCLASS()
class NXPROJECT_API ANXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANXCharacterBase();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> DeathMontage;

	FOnMontageEnded OnDeathMontageEndedDelegate;

	UFUNCTION()
	void OnDeathMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	FTimerHandle DeathTimerHandle;
	FTimerDelegate DeathDelegate;

	//void DestroyCharacter();

private:

	void UpdateHUD();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float NormalSpeeds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultipliers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeeds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int32 AttackCount;



public:
	// -- Getter --
	UFUNCTION(BlueprintCallable)
	float GetNormalSpeed() const;

	UFUNCTION(BlueprintCallable)
	float GetSprintSpeedMultiplier() const;

	UFUNCTION(BlueprintCallable)
	float GetSprintSpeed() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetAttackDamage() const;

	UFUNCTION(BlueprintCallable)
	float GetAttackDelay() const;

	UFUNCTION(BlueprintCallable)
	int32 GetAttackCount() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die();

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	bool bIsDying;
};
