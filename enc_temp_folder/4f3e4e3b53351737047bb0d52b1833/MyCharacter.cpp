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

	CharacterArmsSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterArms"));
	CharacterArmsSkeletalMeshComp->SetupAttachment(SceneComp);

	WeaponSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSKM"));

	WeaponStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSM"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(CharacterArmsSkeletalMeshComp, TEXT("head"));
	CameraComp->SetRelativeLocation(FVector::ZeroVector);

	BaseStatComp = CreateDefaultSubobject<UBaseStatComponent>(TEXT("BaseStatComponent"));

	DamageComp = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

	CharacterAnimInstance = nullptr;
	WeaponAnimInstance = nullptr;

	MoveSound = nullptr;
	JumpingSound = nullptr;
	LandingSound = nullptr;

	MoveState = EMoveState::Idle;
	ActionState = EActionState::Idle;

	bIsMoving = false;
	bIsCrouching = false;
	bIsAiming = false;
	bBugFixFlag = false;
	bIsChangeWeapon = false;

	PlayerController = nullptr;

	EquippedWeapon = nullptr;

	NormalSpeed = 600.0f;
	RunSpeedMultiplier = 1.7f;
	RunSpeed = NormalSpeed * RunSpeedMultiplier;
	StepInterval = 0.44f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponSkeletalMeshComp->AttachToComponent(
		CharacterArmsSkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("ik_hand_gun")
	);

	WeaponStaticMeshComp->AttachToComponent(
		WeaponSkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("magazine")
	);

	if (CharacterArmsSkeletalMeshComp)
	{
		if (CharacterArmsSkeletalMeshComp->SkeletalMesh)
		{
			if (CharacterArmsSkeletalMeshComp->GetAnimInstance())
			{
				CharacterAnimInstance = CharacterArmsSkeletalMeshComp->GetAnimInstance();
			}
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	DamageComp->AttackTokenCount = 1;

	if (GetWorld())
	{
		if (WeaponClasses.Num() > 0)
		{
			SpawnedWeapons.SetNum(WeaponClasses.Num());
			for (int i = 0; i < WeaponClasses.Num(); i++)
			{
				SpawnedWeapons[i] = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClasses[i], FTransform::Identity);
			}
		}

		if (SpawnedWeapons[0])
		{
			WeaponEquip(SpawnedWeapons[0]);
		}

		if (EquippedWeapon)
		{
			if (WeaponSkeletalMeshComp)
			{
				if (WeaponSkeletalMeshComp->SkeletalMesh)
				{
					if (WeaponSkeletalMeshComp->GetAnimInstance())
					{
						WeaponAnimInstance = WeaponSkeletalMeshComp->GetAnimInstance();
					}
				}
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

EActionState AMyCharacter::GetActionState() const
{
	return ActionState;
}

UAnimInstance* AMyCharacter::GetCharacterAnimInstance() const
{
	return CharacterAnimInstance;
}

UAnimInstance* AMyCharacter::GetWeaponAnimInstance() const
{
	return WeaponAnimInstance;
}

void AMyCharacter::WeaponEquip(ABaseWeapon* SpawnedWeapon)
{
	EquippedWeapon = SpawnedWeapon;
	if (EquippedWeapon)
	{
		EquippedWeapon->SetOwner(this);
		EquippedWeapon->Equip();

		if (PlayerController)
		{
			//PlayerController->BindDeligateToSpawnedWeapon(EquippedWeapon);
		}
	}
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
			if (bIsAiming)
			{
				BaseRangedWeapon->SetWeaponState(EWeaponState::Aiming);

				if (MoveState != EMoveState::Running)
				{
					StepInterval *= 2.0f;
				}
			}
			else
			{
				BaseRangedWeapon->SetWeaponState(EWeaponState::Base);
			}
		}
	}
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	if (LandingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			LandingSound,
			CharacterArmsSkeletalMeshComp->GetSocketLocation("root")
		);
	}

	if (bIsCrouching)
	{
		ActionState = EActionState::Crouching;
	}
	else
	{
		ActionState = EActionState::Idle;
	}
}

void AMyCharacter::MoveSoundPlay()
{
	if (!bIsMoving || ActionState == EActionState::Jumping) return;

	if (MoveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			MoveSound,
			CharacterArmsSkeletalMeshComp->GetSocketLocation("root"),
			1.3f
		);
	}

	GetWorldTimerManager().SetTimer(
		MoveSoundTimerHandle,
		this,
		&AMyCharacter::MoveSoundPlay,
		StepInterval,
		false
	);
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
					&AMyCharacter::StartMove
				);

				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopMove
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

			if (PlayerController->WeaponChangeAction)
			{
				EnhancedInput->BindAction(
					PlayerController->WeaponChangeAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::ChangeWeapon
				);
			}
		}
	}
}

void AMyCharacter::StartMove(const FInputActionValue& value)
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

	if (!bIsMoving && ActionState != EActionState::Jumping)
	{
		bIsMoving = true;

		MoveSoundPlay();
	}
}

void AMyCharacter::StopMove(const FInputActionValue& value)
{
	bIsMoving = false;
}

void AMyCharacter::StartRun(const FInputActionValue& value)
{
	if (ActionState != EActionState::Jumping && ActionState != EActionState::Crouching && ActionState != EActionState::Cling)
	{
		if (MoveState != EMoveState::Running && bIsMoving)
		{
			MoveState = EMoveState::Running;

			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			}

			StepInterval *= 0.5f;

			if (Cast<ABaseRangedWeapon>(EquippedWeapon)->GetWeaponState() == EWeaponState::Aiming)
			{
				StepInterval *= 0.5f;
			}
		}
	}
}

void AMyCharacter::StopRun(const FInputActionValue& value)
{
	if (MoveState == EMoveState::Running)
	{
		MoveState = EMoveState::Idle;

		if (ActionState != EActionState::Crouching)
		{
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
			}

			StepInterval *= 2.0f;

			if (Cast<ABaseRangedWeapon>(EquippedWeapon)->GetWeaponState() == EWeaponState::Aiming)
			{
				StepInterval *= 2.0f;
			}
		}
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		bIsMoving = false;	

		ActionState = EActionState::Jumping;

		if (JumpingSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				JumpingSound,
				CharacterArmsSkeletalMeshComp->GetSocketLocation("root")
			);
		}

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
		if (!bIsCrouching)
		{
			if (MoveState == EMoveState::Running)
			{
				MoveState = EMoveState::Idle;

				if (Cast<ABaseRangedWeapon>(EquippedWeapon)->GetWeaponState() == EWeaponState::Aiming)
				{
					StepInterval *= 8.0f;
				}
				else
				{
					StepInterval *= 4.0f;
				}
			}
			else
			{
				StepInterval *= 2.0f;
			}

			ActionState = EActionState::Crouching;

			bIsCrouching = true;

			if (GetCapsuleComponent())
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(44.0f, true);
			}

			if (GetCharacterMovement())
			{
				NormalSpeed *= 0.5f;
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
			}
		}
		else 
		{
			ActionState = EActionState::Idle;

			bIsCrouching = false;

			if (GetCapsuleComponent())
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f, true);
			}

			if (GetCharacterMovement())
			{
				NormalSpeed *= 2.0f;
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

				StepInterval *= 0.5f;
			}
		}
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
				if (BaseRangedWeapon->GetWeaponState() != EWeaponState::Reloading)
				{
					if (MoveState != EMoveState::Running && ActionState != EActionState::Jumping)
					{
						if (BaseRangedWeapon->GetWeaponState() != EWeaponState::Aiming || bBugFixFlag)
						{
							BaseRangedWeapon->SetWeaponState(EWeaponState::Aiming);

							bBugFixFlag = false;
							
							if (!bIsAiming)
							{
								bIsAiming = true;

								StepInterval *= 2.0f;
							}

							if (GetCharacterMovement())
							{
								NormalSpeed *= 0.5f;
								GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
							}
						}

						CameraComp->SetFieldOfView(80.0f);

						OnChangedIsAiming.Broadcast(false);
					}
					else if (MoveState == EMoveState::Running || ActionState == EActionState::Jumping)
					{
						CameraComp->SetFieldOfView(100.0f);

						OnChangedIsAiming.Broadcast(true);
					}
				}
				else if (BaseRangedWeapon->GetWeaponState() == EWeaponState::Reloading)
				{
					if (bIsAiming && !bBugFixFlag)
					{
						if (GetCharacterMovement())
						{
							NormalSpeed *= 2.0f;

							if (MoveState != EMoveState::Running)
							{
								GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
								StepInterval *= 0.5f;
							}
						}

						bBugFixFlag = true;
					}

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
		if (EquippedWeapon)
		{
			if (ABaseRangedWeapon* BaseRangedWeapon = Cast<ABaseRangedWeapon>(EquippedWeapon))
			{
				if (BaseRangedWeapon->GetWeaponState() != EWeaponState::Reloading)
				{
					BaseRangedWeapon->SetWeaponState(EWeaponState::Base);

					if (bIsAiming)
					{
						if (GetCharacterMovement())
						{
							if (!FMath::IsNearlyEqual(NormalSpeed, 600.0f))
							{
								NormalSpeed *= 2.0f;

								if (MoveState != EMoveState::Running)
								{
									GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
								}
							}

							if (MoveState != EMoveState::Running)
							{
								StepInterval *= 0.5f;
							}
						}
					}
				}
			}
		}
	}

	bIsAiming = false;

	CameraComp->SetFieldOfView(100.0f);

	OnChangedIsAiming.Broadcast(true);
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

void AMyCharacter::ChangeWeapon(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (!bIsChangeWeapon)
		{
			bIsChangeWeapon = true;
			if (EquippedWeapon->GetName() == FName("Rifle_1"))
			{
				WeaponEquip(SpawnedWeapons[1]);
			}
			else if (EquippedWeapon->GetName() == FName("Shotgun_1"))
			{
				WeaponEquip(SpawnedWeapons[0]);
			}
		}
		GetWorldTimerManager().SetTimer(
			ChangeWeaponTimerHandle,
			this,
			&AMyCharacter::EndChangeWeapon,
			1.0f,
			false
		);
	}
}

void AMyCharacter::EndChangeWeapon()
{
	bIsChangeWeapon = false;
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

ABaseWeapon* AMyCharacter::GetEquippedWeapon()
{
	return EquippedWeapon;
}
