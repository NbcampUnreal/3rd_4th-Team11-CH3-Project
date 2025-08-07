#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RangedWeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class URangedWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IRangedWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void Shoot() = 0;
	virtual void Reload() = 0;
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) = 0;
};
