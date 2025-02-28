#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "NXNonPlayerCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

class UAnimMonatge;

UCLASS()
class NXPROJECT_API ANXNonPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

public:
	ANXNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void BeginAttack();
	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

	UFUNCTION()
	void OnKnockBackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void EndKnockBack();

	bool bIsNowAttacking;
	bool bIsKnockedBack;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> KnockBackMontage;

	FOnMontageEnded OnKnockBackMontageEndedDelegate;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* CharacterMovementComponent;

	// -- Controller, Service -- 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	float PatrolRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	float DetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack", meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack", meta = (AllowPrivateAccess = "true"))
	AActor* DamageSource;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarClass;

public:
	float GetPatrolRadius() const;
	float GetDetectRadius() const;
	float GetAttackRange() const;
	float GetSphereRadius() const;

	AActor* GetDamageSource() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float HeadShotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float BodyShotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float ArmLegDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float KnockBackStrength;

	void UpdateHealthBar();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;	

	void ApplyKnockBack(FVector Direction, float Strength);
};

