#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponAccCompoenent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UWeaponAccCompoenent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UWeaponAccCompoenent();

	int32 PlusAttack = 0;
	int32 PlusHandle = 100;
	int32 PlusAmmo = 0;


	void UpdateWeaponAcc();
};
