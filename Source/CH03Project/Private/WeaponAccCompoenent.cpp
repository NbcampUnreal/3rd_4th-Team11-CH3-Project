#include "WeaponAccCompoenent.h"
#include "BaseRangedWeapon.h"

UWeaponAccCompoenent::UWeaponAccCompoenent()
{
	PrimaryComponentTick.bCanEverTick = false;
}




void UWeaponAccCompoenent::UpdateWeaponAcc()
{
	if (AActor* Owner = GetOwner())
	{
		if (ABaseRangedWeapon* Weapon = Cast<ABaseRangedWeapon>(Owner))
		{
			Weapon->PlusAttack = PlusAttack;
			Weapon->PlusHandle = PlusHandle;
			Weapon->PlusAmmo = static_cast<float>(PlusAmmo) / 100;
		}
	}
}

