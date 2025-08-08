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
	virtual int32 GetMaxAmmo() = 0;
	virtual float GetShootingRange() = 0;
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) = 0;
	virtual void SetLineTraceStartPoint(FVector StartPoint) = 0;
	virtual void SetLineTraceEndPoint(FVector EndPoint) = 0;
	virtual void Shoot() = 0;
	virtual void Reload() = 0;
};
