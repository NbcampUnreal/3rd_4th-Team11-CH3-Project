// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DefaultAttack.h"
#include "AIController.h"
#include "BaseEnemy.h"

UBTT_DefaultAttack::UBTT_DefaultAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("BTT Default Attack");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Pawn);
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->Attack_Implementation();

	return EBTNodeResult::Succeeded;
}
