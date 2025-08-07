#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponInterface.h"
#include "BaseWeapon.generated.h"

UCLASS()
class CH03PROJECT_API ABaseWeapon : public AActor, public IBaseWeaponInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMeshComp;

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

	UFUNCTION(BlueprintCallable, Category = "BaseWeapon")
	virtual void Attack(AActor* TargetActor) override;
	UFUNCTION(BlueprintCallable, Category = "BaseWeapon")
	virtual void Equip(AActor* TargetActor) override;
	UFUNCTION(BlueprintCallable, Category = "BaseWeapon")
	virtual void Unequip(AActor* TargetActor) override;
	UFUNCTION(BlueprintCallable, Category = "BaseWeapon")
	virtual void ChangeDamage(int32 NewDamage) override;
	UFUNCTION(BlueprintCallable, Category = "BaseWeapon")
	virtual void ChangeAttackSpeed(float NewAttackSpeed) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
