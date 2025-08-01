// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsWeildingWeapon.h"
#include "BaseEnemy.h"

UBTD_IsWeildingWeapon::UBTD_IsWeildingWeapon(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("BTD Is Weilding Weapon");
}

bool UBTD_IsWeildingWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	APawn* ControlledPawn = Controller->GetPawn();
	if (!ControlledPawn) return false;

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(ControlledPawn);
	if (!Enemy) return false;

	return Enemy->bIsWeildingWeapon;
}
