// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DefaultAttack.generated.h"

UCLASS()
class CH03PROJECT_API UBTT_DefaultAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_DefaultAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnAttackFinished();

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
