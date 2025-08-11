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

			CurrentAmmo = MaxAmmo;

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