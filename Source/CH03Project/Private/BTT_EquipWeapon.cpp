// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EquipWeapon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"	
#include "BaseEnemy.h"

UBTT_EquipWeapon::UBTT_EquipWeapon()
{
	NodeName = TEXT("BTT Equip Weapon");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Pawn))
	{
		Enemy->EquipWeapon();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
