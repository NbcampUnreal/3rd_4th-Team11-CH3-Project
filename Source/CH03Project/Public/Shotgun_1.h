#pragma once

#include "CoreMinimal.h"
#include "BaseRangedWeapon.h"
#include "Shotgun_1.generated.h"

UCLASS()
class CH03PROJECT_API AShotgun_1 : public ABaseRangedWeapon
{
	GENERATED_BODY()

protected:
	int32 PelletCount;
	float Spread;

public:
	AShotgun_1();

	virtual void HitScan(AMyCharacter* WeaponOwner) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
