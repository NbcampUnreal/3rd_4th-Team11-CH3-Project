// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UnequipWeapon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BaseEnemy.h"

UBTT_UnequipWeapon::UBTT_UnequipWeapon()
{
	NodeName = TEXT("BTT Unequip Weapon");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_UnequipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		Enemy->UnequipWeapon();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
