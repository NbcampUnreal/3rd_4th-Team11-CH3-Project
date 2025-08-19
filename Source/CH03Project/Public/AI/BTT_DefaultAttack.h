// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DefaultAttack.generated.h"

class AAIController;
class AActor;
class UBehaviorTreeComponent;

UCLASS()
class CH03PROJECT_API UBTT_DefaultAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DefaultAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Reachability") // Check if it's Reachable
	bool bTreatPartialPathAsUnreachable = true;
	UPROPERTY(EditAnywhere, Category = "Reachability", meta = (ClampMin = "0.05", UIMin = "0.05", UIMax = "1.0"))
	float RecheckInterval = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Feasibility") // Check if it's Feasible 
	bool bAllowAttackWithoutPath = true;
	UPROPERTY(EditAnywhere, Category = "Feasibility", meta = (ClampMin = "0.0"))
	float AttackMaxRange = 1200.f;
	UPROPERTY(EditAnywhere, Category = "Feasibility", meta = (ClampMin = "0.0"))
	float MaxVerticalDelta = 500.f;
	UPROPERTY(EditAnywhere, Category = "Feasibility")
	bool bRequireLOS = true;

	UPROPERTY(EditAnywhere, Category = "Attack") // For Recheck
	bool bFailIfMontageNotPlaying = true;
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (ClampMin = "0.1"))
	float AttackTimeout = 4.0f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bBroadcastEndOnAbort = false;

private:
	UFUNCTION()
	void OnAttackFinished();
	bool IsTargetReachable(AAIController* AIController, AActor* Target) const;
	bool IsAttackFeasible(AAIController* AIController, AActor* Target) const;
	void Recheck(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeak);

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	struct FAttackTaskMemory
	{
		FTimerHandle RecheckTimerHandle;
		float        Elapsed = 0.f;
		bool         bStarted = false;
	};
};