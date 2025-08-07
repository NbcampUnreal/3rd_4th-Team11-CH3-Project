// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "EnemyActionInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Delegates/DelegateCombinations.h" 
#include "BaseEnemy.generated.h"

class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class CH03PROJECT_API ABaseEnemy : public ABaseActor, public IEnemyActionInterface
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FVector> TargetPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int32 Direction;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int32 PatrolIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	void IncrementPatrolRoute();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void ShuffleWayPoints();

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackEnd OnAttackEnd;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void CallOnAttackEnd()
	{
		OnAttackEnd.Broadcast();
	}

public:
	virtual TArray<FVector> GetPatrolWaypoints_Implementation() override;
	virtual void SetMovementSpeed_Implementation(ESpeedState SpeedState) override;
	virtual void GetIdealRadius_Implementation(float& OutAttackRadius, float& OutDefendRadius) override;
	virtual void Attack_Implementation(AActor* AttackTarget) override;
};
