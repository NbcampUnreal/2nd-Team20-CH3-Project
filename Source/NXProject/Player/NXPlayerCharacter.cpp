#include "Player/NXPlayerCharacter.h"
#include "Player/NXPlayerController.h"
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

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;


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
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Sprint!"));
	}
}

void ANXPlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
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
	UE_LOG(LogTemp, Warning, TEXT("Crouch!"));
}

void ANXPlayerCharacter::StopCrouch(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch stop!"));
}

void ANXPlayerCharacter::StartReload(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
}

void ANXPlayerCharacter::StopReload(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Reload stop!"));
}
