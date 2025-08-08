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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ShootingRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	FVector EndLocation;


public:	
	ABaseRangedWeapon();

	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual int32 GetMaxAmmo() override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual float GetShootingRange() override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void SetLineTraceStartPoint(FVector StartPoint) override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void SetLineTraceEndPoint(FVector EndPoint) override;
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	virtual void Shoot() override;
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	virtual void Reload() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
