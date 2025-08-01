// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "AIController.h"
#include "BTD_IsWeildingWeapon.generated.h"

UCLASS()
class CH03PROJECT_API UBTD_IsWeildingWeapon : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTD_IsWeildingWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
