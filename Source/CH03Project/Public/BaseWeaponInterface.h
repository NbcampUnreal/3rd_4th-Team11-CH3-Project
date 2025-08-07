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
	virtual void Attack(AActor* TargetActor) = 0;
	virtual void Equip(AActor* TargetActor) = 0;
	virtual void Unequip(AActor* TargetActor) = 0;
	virtual void ChangeDamage(int32 Damage) = 0;
	virtual void ChangeAttackSpeed(float AttackSpeed) = 0;
};
