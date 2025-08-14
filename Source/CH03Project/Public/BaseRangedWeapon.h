#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "RangedWeaponInterface.h"
#include "BaseRangedWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCurrentAmmo, int32, CurrentAmmo);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ReloadingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RecoilPitchScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RecoilYawScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UParticleSystem* ShootHitEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponState WeaponState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* CharacterReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WeaponReloadMontage;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnChangeCurrentAmmo OnChangeCurrentAmmo;

public:	
	ABaseRangedWeapon();

	virtual void Attack() override;
	virtual void Equip() override;
	virtual void Unequip() override;
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	virtual void AddRecoilPitchYaw(float RecoilPitchRange, float RecoilYawRange) override;
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	virtual void Reload() override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual float GetReloadingTime() const override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void ChangeMaxAmmo(int32 NewMaxAmmo) override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual int32 GetCurrnentAmmo() const override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual EWeaponState GetWeaponState() const override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void SetWeaponState(EWeaponState NewState) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	int32 PlusAttack = 0;
	int32 PlusAmmo = 0;
	float PlusHandle = 1;

	void UpdateAttack();
};
