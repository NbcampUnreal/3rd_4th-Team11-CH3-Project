#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.h"
#include "DamageComponent.generated.h"

class IBaseWeaponInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int AttackDamage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Token")
	int32 AttackTokenCount = 0;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetAttackDamage(IBaseWeaponInterface* EquippedWeapon);
	void SetItemPlusAttack(int32 NewItemPlusAttack);
	void SetItemPlusDamage(int32 NewItemPlusDamage); // Overloading
	//int32 GetAttackDamage() const;

	UFUNCTION(BlueprintCallable)
	void TransDamage(AActor* TargetActor);

	UFUNCTION()
	bool StoreAttackToken(int32 Amount);

	UFUNCTION()
	void ReturnAttackToken(int32 Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int ItemPlusDamage;
};
