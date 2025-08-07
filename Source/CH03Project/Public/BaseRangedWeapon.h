#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

UCLASS()
class CH03PROJECT_API ABaseRangedWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	ABaseRangedWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
