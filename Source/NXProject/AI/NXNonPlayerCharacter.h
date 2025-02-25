#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bIsNowAttacking;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;

private:

	// -- Controller, Service -- 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	float PatrolRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	float DetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack", meta = (AllowPrivateAccess = "true"))
	float SphereRadius;


public:
	float GetPatrolRadius() const;
	float GetDetectRadius() const;
	float GetAttackRange() const;
	float GetSphereRadius() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float HeadShotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float BodyShotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GetDamage")
	float ArmLegDamage;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

