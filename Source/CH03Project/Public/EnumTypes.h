// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.generated.h"

UENUM(BlueprintType)
enum class EAISenseType : uint8
{ 
	None	UMETA(DisplayName = "None"),
	Sight	UMETA(DisplayName = "Sight"),
	Hearing	UMETA(DisplayName = "Hearing"),
	Damage	UMETA(DisplayName = "Damage")
};

UENUM(BlueprintType)
enum class ESpeedState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting")
};
