#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NXCharacterBase.generated.h"

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

private:
	// -- 이동 속도 --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float NormalSpeeds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultipliers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeeds;

	// -- 체력 --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	
	// -- 공격 -- 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay;

public:
	// -- Getter --
	float GetNormalSpeed() const;
	float GetSprintSpeedMultiplier() const;
	float GetSprintSpeed() const;
	float GetMaxHealth() const;
	float GetCurrentHealth() const;
	float GetAttackDamage() const;
	float GetAttackDelay() const;

	
	// -- 데미지 처리 --
	// 데미지 양, 데미지 이벤트, 이벤트를 발생시킨 컨트롤러, 데미지를 입힌 액터
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// -- 사망 처리 --
	virtual void Die();

};
