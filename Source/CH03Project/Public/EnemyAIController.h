// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnumTypes.h"
#include "EEnemyState.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UBlackboardComponent;
class UBehaviorTreeComponent;
class ABaseEnemy;
class AActor;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UCLASS()
class CH03PROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY()
	UBlackboardComponent* BB;
	UPROPERTY()
	UBehaviorTreeComponent* BT;
	UPROPERTY()
	ABaseEnemy* ControlledEnemy;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

protected:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	bool CanSenseActor(ABaseEnemy* Enemy, AActor* Actor, const FAIStimulus& Stimulus, FName SenseName) const;

	void HandleSensedSight(AActor* Actor);
	void HandleSensedSound(const FVector& Location);
	void HandleSensedDamage(AActor* Actor);

public:
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetStateAsPassive();
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetStateAsAttacking();
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetStateAsInvestigating(const FVector& Location);

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName TargetActorKey = TEXT("TargetActor");

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName StateKey = TEXT("State");

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName InterestKey = TEXT("PointOfInterest");

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName AttackRadiusKey = TEXT("AttackRadius");

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName DefendRadiusKey = TEXT("DefendRadius");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIBlackboard")
	AActor* TargetActor;

	EEnemyState GetCurrentState() const;
};
