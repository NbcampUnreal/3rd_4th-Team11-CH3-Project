// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UnequipWeapon.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class CH03PROJECT_API UBTT_UnequipWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_UnequipWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
