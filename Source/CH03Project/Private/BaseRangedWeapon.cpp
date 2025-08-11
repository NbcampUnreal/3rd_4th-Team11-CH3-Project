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
	ShootHitEffect = nullptr;
	WeaponState = EWeaponState::Base;
}

void ABaseRangedWeapon::Attack()
{
	if (CurrentAmmo > 0)
	{
		FHitResult Hit;
		FCollisionQueryParams Params;

		if (AMyCharacter* WeaponOwner = Cast<AMyCharacter>(GetOwner()))
		{
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

void ABaseRangedWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;

	OnChangeCurrentAmmo.Broadcast(CurrentAmmo);
}

EWeaponState ABaseRangedWeapon::GetWeaponState()
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

int32 ABaseRangedWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}