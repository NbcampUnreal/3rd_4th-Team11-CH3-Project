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

	int32 plusAttack;
	int32 plusHandle;
	int32 plusExtend;


	void UpdateWeaponAcc();
};
