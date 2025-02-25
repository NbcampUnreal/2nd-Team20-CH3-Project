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
	float GetNormalSpeed() const;
	float GetSprintSpeedMultiplier() const;
	float GetSprintSpeed() const;
	float GetMaxHealth() const;
	float GetCurrentHealth() const;
	float GetAttackDamage() const;
	float GetAttackDelay() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	int32 GetAttackCount() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die();

};
