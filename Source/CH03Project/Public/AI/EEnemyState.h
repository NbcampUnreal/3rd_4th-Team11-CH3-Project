// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Passive			UMETA(DisplayName = "Passive"),
	Attacking		UMETA(DisplayName = "Attacking"),
	Frozen			UMETA(DisplayName = "Frozen"),
	Investigating	UMETA(DisplayName = "Investigating"),
	Dead			UMETA(DisplayName = "Dead")
};