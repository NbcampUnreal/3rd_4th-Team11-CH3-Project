// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/EnemyActionInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Delegates/DelegateCombinations.h" 
#include "BaseEnemy.generated.h"

class AAIController;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class CH03PROJECT_API ABaseEnemy : public ACharacter, public IEnemyActionInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Headshot")
	FName HeadBoneSocketName = "head";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Headshot")
	USphereComponent* HeadCollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Headshot")
	FVector HeadCollisionOffset = FVector::ZeroVector;


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
	void AttachToHeadSocket();
};
