#pragma once

#include "CoreMinimal.h"
#include "BaseRangedWeapon.h"
#include "Rifle_1.generated.h"

UCLASS()
class CH03PROJECT_API ARifle_1 : public ABaseRangedWeapon
{
	GENERATED_BODY()
	
public:	
	ARifle_1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
