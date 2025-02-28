#include "Player/NXCharacterBase.h"
#include "Player/NXPlayerController.h"
#include "Blueprint/UserWidget.h"

ANXCharacterBase::ANXCharacterBase()
	: NormalSpeeds(600.f)
	, SprintSpeedMultipliers(1.5f)
	, SprintSpeeds(NormalSpeeds * SprintSpeedMultipliers)
	, MaxHealth(100.f)
	, CurrentHealth(MaxHealth)
	, AttackDamage(10.f)
	, AttackDelay(1.f)
{
 	
	PrimaryActorTick.bCanEverTick = false;

}


void ANXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANXCharacterBase::UpdateHUD()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ANXPlayerController* NXPC = Cast<ANXPlayerController>(PC);
	if (!NXPC || !NXPC->GetHUDWidget()) return;

	UUserWidget* HUD = NXPC->GetHUDWidget();
	if (!IsValid(HUD)) return; 

	UFunction* UpdateFunction = HUD->FindFunction(FName("UpdateHealthBar"));
	if (UpdateFunction)
	{
		HUD->ProcessEvent(UpdateFunction, nullptr);
	}
}


void ANXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANXCharacterBase::Die()
{

	/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathAnimation)
	{
		AnimInstance->Montage_Play(DeathAnimation);
	}*/

	UE_LOG(LogTemp, Warning, TEXT("AI, Character separate Die Log"));

	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ANXPlayerController* NXPC = Cast<ANXPlayerController>(PC);
		if (NXPC)
		{
			NXPC->SetHUDVisibility(false); 
		}
	}

    SetActorEnableCollision(false);
	Destroy();
}

float ANXCharacterBase::GetNormalSpeed() const
{
	return NormalSpeeds;
}

float ANXCharacterBase::GetSprintSpeedMultiplier() const
{
	return SprintSpeedMultipliers;
}

float ANXCharacterBase::GetSprintSpeed() const
{
	return SprintSpeeds;
}

float ANXCharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

float ANXCharacterBase::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ANXCharacterBase::SetHealth(float NewHealth)
{
	
	if (FMath::IsNearlyEqual(CurrentHealth, NewHealth, KINDA_SMALL_NUMBER))
	{
		return;
	}

	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	UpdateHUD();
}

float ANXCharacterBase::GetAttackDamage() const
{
	return AttackDamage;
}

float ANXCharacterBase::GetAttackDelay() const
{
	return AttackDelay;
}

int32 ANXCharacterBase::GetAttackCount() const
{
	return AttackCount;
}

//float ANXCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	CurrentHealth -= DamageAmount;
//
//	if (CurrentHealth <= 0.f)
//	{
//		Die();  
//	}
//	return DamageAmount;
//}

float ANXCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage <= 0.f) return 0.f;

	SetHealth(CurrentHealth - ActualDamage);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return ActualDamage;
}
