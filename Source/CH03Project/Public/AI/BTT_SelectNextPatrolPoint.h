// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SelectNextPatrolPoint.generated.h"


UCLASS()
class CH03PROJECT_API UBTT_SelectNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_SelectNextPatrolPoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
