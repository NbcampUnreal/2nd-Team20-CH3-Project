#include "Player/NXPlayerCharacter.h"
#include "Player/NXPlayerController.h"
#include "Player/NXWeaponRifle.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	CrouchSpeedMultiplier = 0.5f; // 걷기 속도의 50% (앉기 속도)
	//SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	CrouchSpeed = NormalSpeed * CrouchSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; // 앉기 가능하도록 설정

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
			
			if (PlayerContorller->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerContorller->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ANXPlayerCharacter::StartReload
				);

				EnhancedInput->BindAction(
					PlayerContorller->ReloadAction,
					ETriggerEvent::Completed,
					this,
					&ANXPlayerCharacter::StopReload
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

	UE_LOG(LogTemp, Warning, TEXT("Move!"));
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
		GetCharacterMovement()->MaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Sprint!"));
	}
}

void ANXPlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));
	}
}

void ANXPlayerCharacter::StartAttack(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack!"));
}

void ANXPlayerCharacter::StopAttack(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Stop!"));
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
		GetCharacterMovement()->MaxWalkSpeed;

		UE_LOG(LogTemp, Warning, TEXT("Stand Up! !"));
	}
}

bool ANXPlayerCharacter::GetIsCrouching() const
{
	return bIsCrouched;
}

void ANXPlayerCharacter::StartReload(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}

void ANXPlayerCharacter::StopReload(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload stop!"));
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
	if (IsValid(WeaponInstance)== true)
	{
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;
	}
}