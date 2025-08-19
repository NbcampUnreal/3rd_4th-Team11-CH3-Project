#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RangedWeaponInterface.generated.h"

class AMyCharacter;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Base            UMETA(DisplayName = "Base"),
	Aiming			UMETA(DisplayName = "Aiming"),
	Reloading		UMETA(DisplayName = "Reloading")
};

UINTERFACE(MinimalAPI)
class URangedWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IRangedWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void HitScan(AMyCharacter* WeaponOwner) = 0;
	virtual EWeaponState GetWeaponState() const = 0;
	virtual void SetWeaponState(EWeaponState NewState) = 0;
	virtual void AddRecoilPitchYaw(float RecoilPitchRange, float RecoilYawRange) = 0;
	virtual void Reload() = 0;
	virtual float GetReloadingTime() const = 0;
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) = 0;
	virtual int32 GetCurrnentAmmo() const = 0;
};
