#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "BaseWeapon.h"
#include "BaseRangedWeapon.h"
#include "BaseStatComponent.h"
#include "DamageComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/PickupItem.h"
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

	BaseStatComp = CreateDefaultSubobject<UBaseStatComponent>(TEXT("BaseStatComponent"));

	DamageComp = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

	CharacterAnimInstance = nullptr;
	WeaponAnimInstance = nullptr;

	MoveState = EMoveState::Idle;
	ActionState = EActionState::Idle;

	WeaponClass = nullptr;

	PlayerController = nullptr;

	EquippedWeapon = nullptr;

	NormalSpeed = 600.0f;
	RunSpeedMultiplier = 1.7f;
	RunSpeed = NormalSpeed * RunSpeedMultiplier;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMeshComp1)
	{
		if (SkeletalMeshComp1->GetAnimInstance())
		{
			CharacterAnimInstance = SkeletalMeshComp1->GetAnimInstance();
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	DamageComp->AttackTokenCount = 1;

	if (GetWorld() && WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FTransform::Identity);

		if (EquippedWeapon)
		{
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->Equip();

			if (SkeletalMeshComp2)
			{
				if (SkeletalMeshComp2->GetAnimInstance())
				{
					WeaponAnimInstance = SkeletalMeshComp2->GetAnimInstance();
				}
			}

			if (PlayerController)
			{
				PlayerController->BindDeligateToSpawnedWeapon(EquippedWeapon);
			}
		}
	}
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMyPlayerController* PC = Cast<AMyPlayerController>(NewController);
	if (PC)
	{
		PlayerController = PC;
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMyPlayerController* AMyCharacter::GetMyPlayerController() const
{
	return PlayerController;
}

EMoveState AMyCharacter::GetMoveState() const
{
	return MoveState;
}

UAnimInstance* AMyCharacter::GetCharacterAnimInstance() const
{
	return CharacterAnimInstance;
}

UAnimInstance* AMyCharacter::GetWeaponAnimInstance() const
{
	return WeaponAnimInstance;
}

void AMyCharacter::Attack()
{
	if (!EquippedWeapon) return;

	EquippedWeapon->Attack();
}

void AMyCharacter::EndReload()
{
	if (EquippedWeapon)
	{
		if (ABaseRangedWeapon* BaseRangedWeapon = Cast<ABaseRangedWeapon>(EquippedWeapon))
		{
			BaseRangedWeapon->SetWeaponState(EWeaponState::Base);
		}
	}
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	if (ActionState != EActionState::Crouching)
	{
		ActionState = EActionState::Idle;
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (PlayerController)
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
					ETriggerEvent::Triggered,
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
					&AMyCharacter::StartAttack
				);

				EnhancedInput->BindAction(
					PlayerController->ShootAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopAttack
				);
			}

			if(PlayerController->InteractionAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InteractionAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::OnInteract
				);
			}
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!PlayerController) return;

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
	MoveState = EMoveState::Running;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AMyCharacter::StopRun(const FInputActionValue& value)
{
	MoveState = EMoveState::Idle;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		ActionState = EActionState::Jumping;

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
	if (value.Get<bool>())
	{
		if (EquippedWeapon)
		{
			if (ABaseRangedWeapon* BaseRangedWeapon = Cast<ABaseRangedWeapon>(EquippedWeapon))
			{
				if (MoveState != EMoveState::Running && ActionState != EActionState::Jumping && BaseRangedWeapon->GetWeaponState() != EWeaponState::Reloading)
				{
					BaseRangedWeapon->SetWeaponState(EWeaponState::Aiming);
					CameraComp->SetFieldOfView(80.0f);

					OnChangedIsAiming.Broadcast(false);	
				}
				else if (MoveState == EMoveState::Running || ActionState == EActionState::Jumping)
				{
					BaseRangedWeapon->SetWeaponState(EWeaponState::Base);
					CameraComp->SetFieldOfView(100.0f);

					OnChangedIsAiming.Broadcast(true);
				}
				else if (BaseRangedWeapon->GetWeaponState() == EWeaponState::Reloading)
				{
					BaseRangedWeapon->SetWeaponState(EWeaponState::Reloading);
					CameraComp->SetFieldOfView(100.0f);

					OnChangedIsAiming.Broadcast(true);
				}
			}
		}
	}
}

void AMyCharacter::StopAim(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		Cast<ABaseRangedWeapon>(EquippedWeapon)->SetWeaponState(EWeaponState::Base);
		CameraComp->SetFieldOfView(100.0f);

		OnChangedIsAiming.Broadcast(true);
	}
}

void AMyCharacter::Reload(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (EquippedWeapon)
		{
			if (ABaseRangedWeapon* BaseRangedWeapon = Cast<ABaseRangedWeapon>(EquippedWeapon))
			{
				if (BaseRangedWeapon->GetWeaponState() != EWeaponState::Reloading)
				{
					BaseRangedWeapon->Reload();

					GetWorldTimerManager().SetTimer(
						ReloadTimerHandle,
						this,
						&AMyCharacter::EndReload,
						BaseRangedWeapon->GetReloadingTime(),
						false
					);
				}
			}
		}
	}
}

void AMyCharacter::StartAttack(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Attack();

		if (EquippedWeapon)
		{
			GetWorldTimerManager().SetTimer(
				AttackTimerHandle,
				this,
				&AMyCharacter::Attack,
				EquippedWeapon->GetAttackSpeed(),
				true
			);
		}
	}
}

void AMyCharacter::StopAttack(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void AMyCharacter::OnInteract(const FInputActionValue& value)
{
	FVector StartLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * 88.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel3,
		Params))
	{
		if(APickupItem* PickupItem = Cast<APickupItem>(HitResult.GetActor()))
		{
			PickupItem->Interact();
		}
	}
}




bool AMyCharacter::StoreAttackToken(int32 Amount)
{
	if (DamageComp)
	{
		return DamageComp->StoreAttackToken(Amount);
	}
	return false;
}

void AMyCharacter::ReturnAttackToken(int32 Amount)
{
	if (DamageComp)
	{
		DamageComp->ReturnAttackToken(Amount);
	}
}
