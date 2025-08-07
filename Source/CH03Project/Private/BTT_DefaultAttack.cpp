// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DefaultAttack.h"
#include "EnemyActionInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BaseEnemy.h"

UBTT_DefaultAttack::UBTT_DefaultAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = TEXT("BTT Default Attack");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;

	}

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	CachedOwnerComp = &OwnerComp;

	if (!Enemy->OnAttackEnd.IsAlreadyBound(this, &UBTT_DefaultAttack::OnAttackFinished))
	{
		Enemy->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
	}

	if (Enemy->GetClass()->ImplementsInterface(UEnemyActionInterface::StaticClass()))
	{
		IEnemyActionInterface::Execute_Attack(Enemy, TargetActor);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_DefaultAttack::OnAttackFinished()
{

	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}