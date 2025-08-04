// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BT = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<ABaseEnemy>(InPawn);
	if (!ControlledEnemy) return;
	TargetActor = ControlledEnemy;

	if (ControlledEnemy && ControlledEnemy->BehaviorTree)
	{
		if (UseBlackboard(ControlledEnemy->BehaviorTree->BlackboardAsset, BB))
		{
			RunBehaviorTree(ControlledEnemy->BehaviorTree);

			SetStateAsPassive();

			if (BB)
			{
				float AttackRadius = 0.0f;
				float DefendRadius = 0.0f;
				ControlledEnemy->GetIdealRadius(AttackRadius, DefendRadius);

				BB->SetValueAsFloat(AttackRadiusKey, AttackRadius);
				BB->SetValueAsFloat(DefendRadiusKey, DefendRadius);
			}
		}
	}
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!ControlledEnemy) return;

	const FName Sense = Stimulus.Type.Name;

	if (Sense == UAISense_Sight::StaticClass()->GetFName())
	{
		if (CanSenseActor(ControlledEnemy, Actor, Stimulus, Sense))
		{
			HandleSensedSight(Actor);
		}
	}
	else if (Sense == UAISense_Hearing::StaticClass()->GetFName())
	{
		if (CanSenseActor(ControlledEnemy, Actor, Stimulus, Sense))
		{
			HandleSensedSound(Stimulus.StimulusLocation);
		}
	}
	else if (Sense == UAISense_Damage::StaticClass()->GetFName())
	{
		if (CanSenseActor(ControlledEnemy, Actor, Stimulus, Sense))
		{
			HandleSensedDamage(Actor);
		}
	}
}

bool AEnemyAIController::CanSenseActor(ABaseEnemy* Enemy, AActor* Actor, const FAIStimulus& Stimulus, FName SenseName) const
{
	return (IsValid(Actor) && Stimulus.WasSuccessfullySensed());
}

void AEnemyAIController::HandleSensedSight(AActor* Actor)
{
	if (!BB || !Actor) return;

	if (GetCurrentState() == EEnemyState::Attacking)
	{
		return;
	}

	BB->SetValueAsObject(TargetActorKey, Actor);
	SetStateAsAttacking();
}

void AEnemyAIController::HandleSensedSound(const FVector& Location)
{
	if (!BB) return;

	BB->SetValueAsVector(InterestKey, Location);
	SetStateAsInvestigating(Location);
}

void AEnemyAIController::HandleSensedDamage(AActor* Actor)
{
	if (!BB || !Actor) return;

	if (GetCurrentState() == EEnemyState::Attacking)
	{
		return;
	}

	BB->SetValueAsObject(TargetActorKey, Actor);
	SetStateAsAttacking();
}

void AEnemyAIController::SetStateAsPassive()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(0)); // Enum_EnemyState::Passive
	}
}

void AEnemyAIController::SetStateAsAttacking()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(1)); // Enum_EnemyState::Attacking
	}
}

void AEnemyAIController::SetStateAsInvestigating(const FVector& Location)
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(3)); // Enum_EnemyState::Investigating
		BB->SetValueAsVector(InterestKey, Location);
	}
}

EEnemyState AEnemyAIController::GetCurrentState() const
{
	if (BB)
	{
		const uint8 EnumValue = BB->GetValueAsEnum(StateKey);
		return static_cast<EEnemyState>(EnumValue);
	}

	return EEnemyState::Passive;
}
