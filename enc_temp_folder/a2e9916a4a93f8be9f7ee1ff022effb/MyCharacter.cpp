#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

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
	SkeletalMeshComp2->SetupAttachment(SkeletalMeshComp1);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	StaticMeshComp->SetupAttachment(SkeletalMeshComp2);

	AnimInstance = nullptr;
	FireMontage = nullptr;

	bIsShooting = false;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (SkeletalMeshComp1)
	{
		AnimInstance = SkeletalMeshComp1->GetAnimInstance();
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMyCharacter::GetbIsShooting()
{
	return bIsShooting;
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

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
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

void AMyCharacter::StartShoot(const FInputActionValue& value)
{
	bIsShooting = value.Get<bool>();

	Shoot();

	GetWorldTimerManager().SetTimer(
		ShootTimerHandle,
		this,
		&AMyCharacter::Shoot,
		0.2f,
		true
	);
}

void AMyCharacter::StopShoot(const FInputActionValue& value)
{
	bIsShooting = value.Get<bool>();

	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}
