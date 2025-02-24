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
	// -- �̵� �ӵ� --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float NormalSpeeds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultipliers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float SprintSpeeds;

	// -- ü�� --
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	
	// -- ���� -- 
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
	
	// -- ���� ó�� --
	// ���ذ�, ���� �̺�Ʈ ���� ����(������, �Ѱ� ����), ���ظ� ���� ��ü, ���ظ� ���� ��ü ���⸦ ���� ����.
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// -- ��� ó�� -- 
	virtual void Die();
};
