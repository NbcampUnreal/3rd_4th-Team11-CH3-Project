// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SelectNextPatrolPoint.h"
#include "AIController.h"
#include "BaseEnemy.h"

UBTT_SelectNextPatrolPoint::UBTT_SelectNextPatrolPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("BTT Select Next Patrol Point");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_SelectNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		Enemy->IncrementPatrolRoute();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
