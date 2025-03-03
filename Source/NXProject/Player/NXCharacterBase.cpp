#include "Player/NXCharacterBase.h"
#include "Player/NXPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

ANXCharacterBase::ANXCharacterBase()
	: NormalSpeeds(600.f)
	, SprintSpeedMultipliers(1.5f)
	, SprintSpeeds(NormalSpeeds * SprintSpeedMultipliers)
	, MaxHealth(100.f)
	, CurrentHealth(MaxHealth)
	, AttackDamage(10.f)
	, AttackDelay(1.f)
	, bIsDying(false)
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
	if (bIsDying)  
	{
		UE_LOG(LogTemp, Warning, TEXT("[Die] 이미 사망 처리 중, 중복 실행 방지"));
		return;
	}
	bIsDying = true;  

	UE_LOG(LogTemp, Error, TEXT("[Die] Die 함수 호출됨!"));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		bool bPlayed = AnimInstance->Montage_Play(DeathMontage) > 0.0f;

		if (!bPlayed)
		{
			UE_LOG(LogTemp, Error, TEXT("Die(): Death Montage did not play!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Death montage is playing!"));

			//float MontageDuration = DeathMontage->GetPlayLength();
			//GetWorldTimerManager().SetTimer(DeathTimerHandle, DeathDelegate, MontageDuration, false);

			


			OnDeathMontageEndedDelegate.BindUObject(this, &ANXCharacterBase::OnDeathMontageEnd);
			AnimInstance->Montage_SetEndDelegate(OnDeathMontageEndedDelegate, DeathMontage);

			UE_LOG(LogTemp, Warning, TEXT("DeathEnd!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Die(): AnimInstance or DeathMontage is NULL!"));
		OnDeathMontageEnd(nullptr, false);
	}

	TObjectPtr<AController> MyController = GetController();
	if (MyController)
	{
		MyController->UnPossess();
	}

	if (APlayerController* PC = Cast<APlayerController>(MyController))
	{
		if (ANXPlayerController* NXPC = Cast<ANXPlayerController>(PC))
		{
			NXPC->SetHUDVisibility(false);
		}
	}
	SetLifeSpan(3.0f);  
}


void ANXCharacterBase::OnDeathMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeathMontageEnd called"));


	if (Montage == DeathMontage)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->DisableMovement();   
			GetCharacterMovement()->StopMovementImmediately(); 
		}

		bIsDying = false;  

		UE_LOG(LogTemp, Warning, TEXT("Death animation finished, destroying actor."));


		UE_LOG(LogTemp, Warning, TEXT("Setting LifeSpan to 3.0f for actor."));
		SetLifeSpan(3.0f);
	}
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
