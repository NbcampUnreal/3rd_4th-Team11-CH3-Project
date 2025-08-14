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
			/*Weapon->plusAttack += plusAttack;
			Weapon->plusHandle += plusHandle;
			Weapon->plusExtend += plusExtend;*/
		}
	}
}

