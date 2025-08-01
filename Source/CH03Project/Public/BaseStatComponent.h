#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UBaseStatComponent();

protected:
	virtual void BeginPlay() override;

	
	//체력관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStat")
	int Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStat")
	int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStat")
	int Armor;

	//대미지면역관리
	bool bIsImmuneToDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStat")
	float ImmuneToDamageTime;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddHp(int Point);
	void AddMaxHp(int Point);

	void AddArmor(int Point);

	void ImmuneToDamageSet();
	void ImmuneToDamageEnd();

	virtual void OnDeath();
};
