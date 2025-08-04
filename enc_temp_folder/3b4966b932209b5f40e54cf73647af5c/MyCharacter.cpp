#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	SceneComp->SetupAttachment(GetCapsuleComponent());

	SkeletalMeshComp1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterArms"));
	SkeletalMeshComp1->SetupAttachment(SceneComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SkeletalMeshComp1, TEXT("head"));
	CameraComp->SetRelativeLocation(FVector::ZeroVector);

	SkeletalMeshComp2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));

	AnimInstance = nullptr;
	FireMontage = nullptr;

	CharacterState = ECharacterState::Idle;

	NormalSpeed = 400.0f;
	RunSpeedMultiplier = 2.0f;
	RunSpeed = NormalSpeed * RunSpeedMultiplier;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMeshComp2->AttachToComponent(
		SkeletalMeshComp1,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("ik_hand_gun")
	);

	StaticMeshComp->AttachToComponent(
		SkeletalMeshComp2,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("magazine")
	);

	if (SkeletalMeshComp1)
	{
		AnimInstance = SkeletalMeshComp1->GetAnimInstance();
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::Shoot()
{
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage, 1.0f);
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

			if (PlayerController->RunAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RunAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartRun
				);

				EnhancedInput->BindAction(
					PlayerController->RunAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopRun
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::Crouch
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}

			if (PlayerController->AimAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AimAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::StartAim
				);

				EnhancedInput->BindAction(
					PlayerController->AimAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopAim
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::Reload
				);
			}

			if (PlayerController->ShootAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ShootAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::StartShoot
				);

				EnhancedInput->BindAction(
					PlayerController->ShootAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopShoot
				);
			}
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.Y);
	}

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.X);
	}
}

void AMyCharacter::StartRun(const FInputActionValue& value)
{
	CharacterState = ECharacterState::Running;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AMyCharacter::StopRun(const FInputActionValue& value)
{
	CharacterState = ECharacterState::Idle;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::Crouch(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{

	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

	if (SceneComp)
	{
		FRotator CurrentRotation = SceneComp->GetRelativeRotation();
		float NewPitch = CurrentRotation.Pitch - LookInput.Y;
		NewPitch = FMath::ClampAngle(NewPitch, -80.f, 80.f);

		SceneComp->SetRelativeRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));
	}
}

void AMyCharacter::StartAim(const FInputActionValue& value)
{
	if (CharacterState != ECharacterState::Running)
	{
		if (value.Get<bool>())
		{
			CharacterState = ECharacterState::Aiming;
		}
	}
}

void AMyCharacter::StopAim(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		CharacterState = ECharacterState::Idle;
	}
}

void AMyCharacter::Reload(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		
	}
}

void AMyCharacter::StartShoot(const FInputActionValue& value)
{
	if (CharacterState != ECharacterState::Running)
	{
		if (value.Get<bool>())
		{
			Shoot();

			GetWorldTimerManager().SetTimer(
				ShootTimerHandle,
				this,
				&AMyCharacter::Shoot,
				0.2f,
				true
			);
		}
	}
}

void AMyCharacter::StopShoot(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		GetWorldTimerManager().ClearTimer(ShootTimerHandle);
	}
}
