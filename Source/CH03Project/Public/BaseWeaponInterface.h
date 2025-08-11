#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseWeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBaseWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IBaseWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void Attack() = 0;
	virtual void Equip() = 0;
	virtual void Unequip() = 0;
	virtual FName GetWeaponType() = 0;
	virtual FName GetWeaponName() = 0;
	virtual int32 GetDamage() = 0;
	virtual float GetAttackSpeed() = 0;
	virtual void ChangeDamage(int32 Damage) = 0;
	virtual void ChangeAttackSpeed(float AttackSpeed) = 0;
};
