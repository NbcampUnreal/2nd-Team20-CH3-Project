#include "Player/NXPlayerCharacter.h"
#include "Player/NXPlayerController.h"
#include "Player/NXWeaponRifle.h"
#include "AI/NXNonPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"

ANXPlayerCharacter::ANXPlayerCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->TargetArmLength = 300.0f;

	SpringArmComp->bUsePawnControlRotation = true;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	CameraComp->bUsePawnControlRotation = false;





    float NormalSpeed = GetNormalSpeed();
	float SprintSpeedMultiplier = GetSprintSpeedMultiplier();
	float SprintSpeed = GetSprintSpeed();
	CrouchSpeedMultiplier = 0.5f; 
	CrouchSpeed = NormalSpeed * CrouchSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; 

}

void ANXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	float MaxHP = GetMaxHealth();
	float CurrentHP = GetCurrentHealth();
	float Damage = GetAttackDamage();
	float Delay = GetAttackDelay();

	UE_LOG(LogTemp, Warning, TEXT("Player Max Health: %f, Current Health: %f, Attack Damage: %f, Attack Delay: %f"),
		MaxHP, CurrentHP, Damage, Delay);


	WeaponActor = GetWorld()->SpawnActor<ANXWeaponRifle>(Weapon);
	if (IsValid(WeaponActor))
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
		WeaponActor->SetOwner(this);
	}
}

void ANXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ANXPlayerController* PlayerContorller = Cast<ANXPlayerController>(GetController()))
		{
			if (PlayerContorller->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::Move
				);
			}

			if (PlayerContorller->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerContorller->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopJump
				);
			}

			if (PlayerContorller->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::Look
				);
			}

			if (PlayerContorller->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerContorller->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopSprint
				);
			}

			if (PlayerContorller->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartCrouch
				);

				EnhancedInput->BindAction(
					PlayerContorller->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopCrouch
				);
			}

			
			if (PlayerContorller->AttackAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->AttackAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartAttack 
				);

				EnhancedInput->BindAction(
					PlayerContorller->AttackAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopAttack
				);

			}


			if (PlayerContorller->PunchAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->PunchAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartPunchAttack 
				);

				EnhancedInput->BindAction(
					PlayerContorller->PunchAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopPunchAttack
				);

			}
				
			if (PlayerContorller->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::Reload
				);
			}

			if (PlayerContorller->QuickSlot01)
			{
				EnhancedInput->BindAction(
					PlayerContorller->QuickSlot01,
					ETriggerEvent::Started,
					this,
					&ThisClass::InputQuickSlot01
				);
			}

			if (PlayerContorller->QuickSlot02)
			{
				EnhancedInput->BindAction(
					PlayerContorller->QuickSlot02,
					ETriggerEvent::Started,
					this,
					&ThisClass::InputQuickSlot02
				);
			}
			
		
		}
	}
}

void ANXPlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Move!"));
}

void ANXPlayerCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ANXPlayerCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ANXPlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);

	AddControllerPitchInput(LookInput.Y);
}

void ANXPlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetSprintSpeed();
		UE_LOG(LogTemp, Warning, TEXT("Sprint!"));
	}
}

void ANXPlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetNormalSpeed();
		UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));

	}
}


void ANXPlayerCharacter::StartPunchAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("🔹 StartPunchAttack() 호출됨"));

	PlayMeleeAttackAnimation();

	FVector PlayerLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		this,
		PlayerLocation + (ForwardVector * (MeleeAttackRange * 0.5f)),
		MeleeAttackRange,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		ANXNonPlayerCharacter::StaticClass(),
		TArray<AActor*>(),
		OverlappingActors
	);

	for (AActor* Actor : OverlappingActors)
	{
		ANXNonPlayerCharacter* Enemy = Cast<ANXNonPlayerCharacter>(Actor);
		if (Enemy)
		{
			float DamageAmount = MeleeDamage;
			FDamageEvent DamageEvent;

			Enemy->TakeDamage(DamageAmount, DamageEvent, GetController(), this);

			UE_LOG(LogTemp, Warning, TEXT("근접 공격 성공! AI에게 %f의 데미지 적용"), DamageAmount);
		}
	}

	DrawDebugSphere(GetWorld(), PlayerLocation + (ForwardVector * (MeleeAttackRange * 0.5f)), MeleeAttackRange, 12, FColor::Red, false, 1.0f);
}

void ANXPlayerCharacter::StopPunchAttack(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Punch Stop!"));
}

void ANXPlayerCharacter::StartAttack()
{
	if (WeaponActor) 
	{
		WeaponActor->Fire();
		
	}


	/*FVector Start = CameraComp->GetComponentLocation();
	FVector ForwardVector = CameraComp->GetForwardVector();
	FVector End = Start + (ForwardVector * this->RangedAttackRange); 

	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); 

	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 2.0f); 

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			
			ANXNonPlayerCharacter* Enemy = Cast<ANXNonPlayerCharacter>(HitActor);
			if (Enemy)
			{
				float DamageAmount = this->GetAttackDamage();

				FDamageEvent DamageEvent;
				Enemy->TakeDamage(DamageAmount, DamageEvent,this-> GetController(), this);

				UE_LOG(LogTemp, Warning, TEXT("Damage : %f, Target: %s"), DamageAmount, *Enemy->GetName());
			}
		}
	}*/
}

void ANXPlayerCharacter::StopAttack(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Stop!"));
}

void ANXPlayerCharacter::Fire(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));

	StartAttack();
}


void ANXPlayerCharacter::StartCrouch(const FInputActionValue& value)
{
	if (value.Get<bool>() && CanCrouch())
	{
		Crouch();
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

		UE_LOG(LogTemp, Warning, TEXT("Crouching !"));
	}
}

void ANXPlayerCharacter::StopCrouch(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed = GetNormalSpeed();

		UE_LOG(LogTemp, Warning, TEXT("Stand Up! !"));
	}
}

bool ANXPlayerCharacter::GetIsCrouching() const
{
	return bIsCrouched;
}

void ANXPlayerCharacter::Reload(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}

void ANXPlayerCharacter::InputQuickSlot01(const FInputActionValue& InValue)
{
	FName WeaponSocket(TEXT("WeaponSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		WeaponInstance = GetWorld()->SpawnActor<ANXWeaponRifle>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}
}

void ANXPlayerCharacter::InputQuickSlot02(const FInputActionValue& Invalue)
{
	if (IsValid(WeaponInstance) == true)
	{
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;
	}
}

void ANXPlayerCharacter::PlayMeleeAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance가 유효하지 않음! 애니메이션 실행 불가"));
		return;
	}

	
	if (!IsValid(MeleeAttackMontage))
	{
		UE_LOG(LogTemp, Error, TEXT("MeleeAttackMontage가 설정되지 않음!"));
		return;
	}

	
	if (!AnimInstance->Montage_IsPlaying(MeleeAttackMontage))
	{
		AnimInstance->Montage_Play(MeleeAttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("근접 공격 애니메이션 실행됨!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("근접 공격 애니메이션 이미 실행 중"));
	}
}

