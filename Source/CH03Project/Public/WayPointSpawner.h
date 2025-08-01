// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayPointSpawner.generated.h"

class AWayPoint;
class ABaseEnemy;

UCLASS()
class CH03PROJECT_API AWayPointSpawner : public AActor
{
	GENERATED_BODY()

public:
	AWayPointSpawner();

protected:
	UPROPERTY(EditAnywhere, Category = "Waypoint")
	float SampRadius;

	UPROPERTY(EditAnywhere, Category = "Waypoint")
	float WallCheckRadius;

	UPROPERTY(EditAnywhere, Category = "Waypoint")
	int32 MaxAttemptsPerEnemy;

	UPROPERTY(EditAnywhere, Category = "Waypoint")
	int32 NumWayPoints;

public:
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void GenerateWaypointVectorsForActor(ABaseEnemy* Enemy, TArray<FVector>& OutVectors);
};