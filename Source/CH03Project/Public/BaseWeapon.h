#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponInterface.h"
#include "BaseWeapon.generated.h"

UCLASS()
class CH03PROJECT_API ABaseWeapon : public AActor, public IBaseWeaponInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMeshComp;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type")
	FName WeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Name")
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;

public:	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Bihavior")
	virtual void Attack() override;
	UFUNCTION(BlueprintCallable, Category = "Bihavior")
	virtual void Equip() override;
	UFUNCTION(BlueprintCallable, Category = "Bihavior")
	virtual void Unequip() override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual FName GetWeaponType() const override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual FName GetWeaponName() const override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual int32 GetDamage() const override;
	UFUNCTION(BlueprintCallable, Category = "Get")
	virtual float GetAttackSpeed() const override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void ChangeDamage(int32 NewDamage) override;
	UFUNCTION(BlueprintCallable, Category = "Set")
	virtual void ChangeAttackSpeed(float NewAttackSpeed) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
