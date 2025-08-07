#include "BaseRangedWeapon.h"

ABaseRangedWeapon::ABaseRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxAmmo = 0;
	CurrentAmmo = 0;
}

void ABaseRangedWeapon::Shoot()
{

}

void ABaseRangedWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void ABaseRangedWeapon::ChangeMaxAmmo(int32 NewMaxAmmo)
{
	MaxAmmo = NewMaxAmmo;
}

void ABaseRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

