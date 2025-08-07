#include "BaseRangedWeapon.h"

ABaseRangedWeapon::ABaseRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

