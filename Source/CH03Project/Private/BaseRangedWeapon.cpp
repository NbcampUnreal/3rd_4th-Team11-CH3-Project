#include "BaseRangedWeapon.h"

ABaseRangedWeapon::ABaseRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponType = "RangedWeapon";
	MaxAmmo = 0;
	CurrentAmmo = 0;
	ShootingRange = 0.0f;
	StartLocation = FVector::ZeroVector;
	EndLocation = FVector::ZeroVector;
}

int32 ABaseRangedWeapon::GetMaxAmmo()
{
	return MaxAmmo;
}

float ABaseRangedWeapon::GetShootingRange()
{
	return ShootingRange;
}

void ABaseRangedWeapon::ChangeMaxAmmo(int32 NewMaxAmmo)
{
	MaxAmmo = NewMaxAmmo;
}

void ABaseRangedWeapon::SetLineTraceStartPoint(FVector StartPoint)
{
	StartLocation = StartPoint;
}

void ABaseRangedWeapon::SetLineTraceEndPoint(FVector EndPoint)
{
	EndLocation = EndPoint;
}

void ABaseRangedWeapon::Shoot()
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECC_GameTraceChannel2,
		Params
	);
	/*
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
			if (DamageComp)
			{
				DamageComp->TransDamage(HitActor);
			}
		}
	}
	*/
}

void ABaseRangedWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void ABaseRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

