// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveAlongWayPoints.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class CH03PROJECT_API UBTT_MoveAlongWayPoints : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveAlongWayPoints(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
