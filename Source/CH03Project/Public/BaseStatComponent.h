#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHpChangedSignature, int32, OldHp, int32, NewHp, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStat")
	int32 KillScore;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddHp(int Point);
	void AddMaxHp(int Point);
	void HealHP(float Point); // 체력회복 포션에서 사용

	void AddArmor(int Point);
	void SetArmor(int Point);

	void ImmuneToDamageSet();
	void ImmuneToDamageEnd();

	virtual void OnDeath();

	int GetHp();
	int GetMaxHp();
	int GetArmor();

	UPROPERTY(BlueprintCallable)
	FOnHpChangedSignature OnHpChangedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeathEvent;

private:
	bool bIsDead;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName ShouldStunBBKeyName = TEXT("ShouldStun");

	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	bool bTriggeredStun70 = false;

	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	bool bTriggeredStun38 = false;

	void RaiseStunOnAI();
};
