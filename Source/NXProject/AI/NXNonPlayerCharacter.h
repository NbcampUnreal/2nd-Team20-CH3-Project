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


public:
	// -- private Getter ÇÔ¼ö --
	float GetPatrolRadius() const;
	float GetDetectRadius() const;
	float GetAttackRange() const;
};

