// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MoveAlongWayPoints.h"
#include "AIController.h"
#include "BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveAlongWayPoints::UBTT_MoveAlongWayPoints(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("BTT Move Along Way Points");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_MoveAlongWayPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	TArray<FVector> WayPoints = Enemy->GetPatrolWaypoints_Implementation();
	int32 CurrentIndex = Enemy->PatrolIndex;
	if (WayPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		if (i == CurrentIndex)
		{
			const FVector& TargetLocation = WayPoints[Enemy->PatrolIndex];

			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(TargetLocation);
			MoveRequest.SetAcceptanceRadius(10.0f);

			FNavPathSharedPtr NavPath;
			FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest, &NavPath);

			Enemy->ShuffleWayPoints();

			if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_MoveAlongWayPoints::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AIController->StopMovement();
	}
	return EBTNodeResult::Aborted;
}
