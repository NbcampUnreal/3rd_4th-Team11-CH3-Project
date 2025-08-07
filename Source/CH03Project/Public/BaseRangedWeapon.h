#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "RangedWeaponInterface.h"
#include "BaseRangedWeapon.generated.h"

UCLASS()
class CH03PROJECT_API ABaseRangedWeapon : public ABaseWeapon, public IRangedWeaponInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentAmmo;

public:	
	ABaseRangedWeapon();

	UFUNCTION(BlueprintCallable, Category = "BaseRangedWeapon")
	virtual void Shoot() override;
	UFUNCTION(BlueprintCallable, Category = "BaseRangedWeapon")
	virtual void Reload() override;
	UFUNCTION(BlueprintCallable, Category = "BaseRangedWeapon")
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
