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
			UE_LOG(LogTemp, Warning, TEXT("UpdateWeaponAcc called for %s"), *Weapon->GetName());
			//전달하는 매개들의 수치
			UE_LOG(LogTemp, Warning, TEXT("PlusAttack: %d, PlusHandle: %d, PlusAmmo: %d"), PlusAttack, PlusHandle, PlusAmmo);
			Weapon->PlusAttack = PlusAttack;
			Weapon->PlusHandle = static_cast<float>(PlusHandle) / 100;
			Weapon->PlusAmmo = PlusAmmo;
			Weapon->UpdateAttack();
		}
	}
}

