// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EquipWeapon.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class CH03PROJECT_API UBTT_EquipWeapon : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_EquipWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
