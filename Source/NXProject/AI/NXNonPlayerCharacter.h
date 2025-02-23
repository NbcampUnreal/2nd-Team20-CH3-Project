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
	
};
