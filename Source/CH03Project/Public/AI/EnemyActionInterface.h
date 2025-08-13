// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumTypes.h"
#include "GameFramework/Actor.h"
#include "EnemyActionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyActionInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IEnemyActionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Patrol")
	TArray<FVector> GetPatrolWaypoints();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Movement")
	void SetMovementSpeed(ESpeedState SpeedState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void GetIdealRadius(float& OutAttackRadius, float& OutDefendRadius);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Combat")
	void Attack(AActor* AttackTarget);
};
