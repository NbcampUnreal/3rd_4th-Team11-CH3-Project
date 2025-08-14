#include "BaseRangedWeapon.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "DamageComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseRangedWeapon::ABaseRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponType = "RangedWeapon";
	MaxAmmo = 0;
	CurrentAmmo = 0;
	ShootingRange = 0.0f;
	ReloadingTime = 0.0f;
	RecoilPitchScale = 0.0f;
	RecoilYawScale = 0.0f;
	ShootHitEffect = nullptr;
	WeaponState = EWeaponState::Base;
	CharacterFireMontage = nullptr;
	CharacterReloadMontage = nullptr;
	WeaponReloadMontage = nullptr;
}

void ABaseRangedWeapon::Attack()
{
	if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
	{
		if (WeaponOwner->GetMoveState() != EMoveState::Running)
		{
			if (WeaponState != EWeaponState::Reloading)
			{
				if (CurrentAmmo > 0)
				{
					if (WeaponOwner->GetCharacterAnimInstance() && CharacterFireMontage)
					{
						WeaponOwner->GetCharacterAnimInstance()->Montage_Play(CharacterFireMontage, 1.0f);

						FHitResult Hit;
						FCollisionQueryParams Params;

						Params.AddIgnoredActor(WeaponOwner);

						FVector Location;
						FRotator Rotation;
						WeaponOwner->GetMyPlayerController()->GetPlayerViewPoint(Location, Rotation);
						FVector End = Location + (Rotation.Vector() * ShootingRange);

						bool bHit = GetWorld()->LineTraceSingleByChannel(
							Hit,
							Location,
							End,
							ECC_GameTraceChannel2,
							Params
						);

						if (ShootHitEffect)
						{
							UGameplayStatics::SpawnEmitterAtLocation(
								GetWorld(),
								ShootHitEffect,
								Hit.ImpactPoint,
								Hit.ImpactNormal.Rotation()
							);
						}

						if (bHit)
						{
							AActor* HitActor = Hit.GetActor();

							if (HitActor && HitActor->ActorHasTag("Enemy"))
							{
								if (UDamageComponent* OwnerDamageComp = WeaponOwner->DamageComp)
								{
									OwnerDamageComp->TransDamage(HitActor);
								}
							}
						}
					}

					AddRecoilPitchYaw(0.5f, 0.1f);

					CurrentAmmo--;

					OnChangeCurrentAmmo.Broadcast(CurrentAmmo);
				}
			}
		}
	}
}

void ABaseRangedWeapon::Equip()
{
	if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
	{
		if (IsValid(SkeletalMeshComp))
		{
			if (IsValid(SkeletalMeshComp->SkeletalMesh))
			{
				WeaponOwner->SkeletalMeshComp2->SetSkeletalMesh(SkeletalMeshComp->SkeletalMesh);
			}
		}

		if (IsValid(StaticMeshComp))
		{
			if (IsValid(StaticMeshComp->GetStaticMesh()))
			{
				WeaponOwner->StaticMeshComp->SetStaticMesh(StaticMeshComp->GetStaticMesh());
			}
		}

		WeaponOwner->SkeletalMeshComp2->AttachToComponent(
			WeaponOwner->SkeletalMeshComp1,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("ik_hand_gun")
		);

		WeaponOwner->StaticMeshComp->AttachToComponent(
			WeaponOwner->SkeletalMeshComp2,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("magazine")
		);

		WeaponOwner->DamageComp->SetAttackDamage(this);
	}
}

void ABaseRangedWeapon::Unequip()
{

}

void ABaseRangedWeapon::AddRecoilPitchYaw(float RecoilPitchRange, float RecoilYawRange)
{
	if (IsValid(GetOwner()))
	{
		if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
		{
			float RandomRecoilPitchScale = FMath::FRandRange(RecoilPitchScale - RecoilPitchRange, RecoilPitchScale + RecoilPitchRange);
			float RandomRecoilYawScale = FMath::FRandRange(RecoilYawScale - RecoilYawRange, RecoilYawScale + RecoilYawRange);
			if (FMath::RandRange(0, 1) == 0)
			{
				RandomRecoilYawScale *= -1.0f;
			}
			
			if (WeaponOwner->GetActionState() == EActionState::Crouching)
			{
				RandomRecoilPitchScale *= 0.7f;
				RandomRecoilYawScale *= 0.7f;
			}
			else if (WeaponOwner->GetActionState() == EActionState::Jumping)
			{
				RandomRecoilPitchScale *= 1.5f;
				RandomRecoilYawScale *= 1.5f;
			}
			if (WeaponOwner->GetActionState() != EActionState::Jumping && WeaponState == EWeaponState::Aiming)
			{
				RandomRecoilPitchScale *= 0.7f;
				RandomRecoilYawScale *= 0.7f;
			}

			WeaponOwner->AddControllerPitchInput(RandomRecoilPitchScale * PlusHandle);
			WeaponOwner->AddControllerYawInput(RandomRecoilYawScale * PlusHandle);

			FRotator CurrentRotation = WeaponOwner->SceneComp->GetRelativeRotation();
			float NewPitch = CurrentRotation.Pitch + RandomRecoilPitchScale;
			float NewYaw = CurrentRotation.Yaw + RandomRecoilYawScale;
			NewPitch = FMath::ClampAngle(NewPitch, NewPitch, 80.0f);
			NewYaw = FMath::ClampAngle(NewYaw, -30.0f, 30.0f);

			WeaponOwner->SceneComp->SetRelativeRotation(FRotator(NewPitch, NewYaw, CurrentRotation.Roll));
		}
	}
}

void ABaseRangedWeapon::ChangeMaxAmmo(int32 NewMaxAmmo)
{
	MaxAmmo = NewMaxAmmo;
}

int32 ABaseRangedWeapon::GetCurrnentAmmo() const
{
	return CurrentAmmo;
}

void ABaseRangedWeapon::Reload()
{
	if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
	{
		if (WeaponOwner->GetCharacterAnimInstance() && CharacterReloadMontage && WeaponOwner->GetWeaponAnimInstance() && WeaponReloadMontage)
		{
			WeaponState = EWeaponState::Reloading;

			WeaponOwner->GetCharacterAnimInstance()->Montage_Play(CharacterReloadMontage, 1.0f);
			WeaponOwner->GetWeaponAnimInstance()->Montage_Play(WeaponReloadMontage, 1.0f);

			CurrentAmmo = MaxAmmo + PlusAmmo;

			OnChangeCurrentAmmo.Broadcast(CurrentAmmo);
		}
	}
}

float ABaseRangedWeapon::GetReloadingTime() const
{
	return ReloadingTime;
}

EWeaponState ABaseRangedWeapon::GetWeaponState() const
{
	return WeaponState;
}

void ABaseRangedWeapon::SetWeaponState(EWeaponState NewState)
{
	WeaponState = NewState;
}

void ABaseRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseRangedWeapon::UpdateAttack()
{
	if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
	{
		WeaponOwner->DamageComp->SetItemPlusAttack(Damage + PlusAttack);
	}
}