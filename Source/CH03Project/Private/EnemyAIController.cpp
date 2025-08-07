// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BT = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig->HearingRange = 600.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->ConfigureSense(*HearingConfig);
	AIPerception->ConfigureSense(*DamageConfig);

	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	StateKey = "State";
	TargetActorKey = "TargetActor";
	InterestKey = "PointOfInterest";
	AttackRadiusKey = "AttackRadius";
	DefendRadiusKey = "DefendRadius";
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

	if (ControlledEnemy && ControlledEnemy->BehaviorTree)
	{
		if (UseBlackboard(ControlledEnemy->BehaviorTree->BlackboardAsset, BB))
		{
			RunBehaviorTree(ControlledEnemy->BehaviorTree);

			if (BB)
			{
				float AttackRadius = 150.0f;
				float DefendRadius = 350.0f;
				if (ControlledEnemy->GetClass()->ImplementsInterface(UEnemyActionInterface::StaticClass()))
				{
					IEnemyActionInterface::Execute_GetIdealRadius(ControlledEnemy, AttackRadius, DefendRadius);
				}

				BB->SetValueAsFloat(AttackRadiusKey, AttackRadius);
				BB->SetValueAsFloat(DefendRadiusKey, DefendRadius);
			}

			SetStateAsPassive();
		}
	}
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!ControlledEnemy || !AIPerception) return;

	TArray<AActor*> UpdatedActors;
	AIPerception->GetCurrentlyPerceivedActors(NULL, UpdatedActors);

	for (AActor* PerceivedActor : UpdatedActors)
	{
		if (!IsValid(PerceivedActor))
			continue;

		TSubclassOf<UAISense> SensedClass;

		if (CanSenseActor(ControlledEnemy, PerceivedActor, UAISense_Sight::StaticClass(), SensedClass))
		{
			HandleSensedSight(PerceivedActor);
			continue;
		}

		if (CanSenseActor(ControlledEnemy, PerceivedActor, UAISense_Hearing::StaticClass(), SensedClass))
		{
			FActorPerceptionBlueprintInfo PerceptionInfo;
			AIPerception->GetActorsPerception(PerceivedActor, PerceptionInfo);

			for (const FAIStimulus& EachStimulus : PerceptionInfo.LastSensedStimuli)
			{
				if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), EachStimulus) == UAISense_Hearing::StaticClass())
				{
					HandleSensedSound(EachStimulus.StimulusLocation);
					break;
				}
			}
			continue;
		}

		if (CanSenseActor(ControlledEnemy, PerceivedActor, UAISense_Damage::StaticClass(), SensedClass))
		{
			HandleSensedDamage(PerceivedActor);
		}
	}
}


bool AEnemyAIController::CanSenseActor(ABaseEnemy* Enemy, AActor* Actor,
	TSubclassOf<UAISense> SenseToCheck,
	TSubclassOf<UAISense>& OutSensedClass) const
{
	if (!IsValid(Actor) || !AIPerception)
	{
		return false;
	}

	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerception->GetActorsPerception(Actor, PerceptionInfo);

	for (const FAIStimulus& EachStimulus : PerceptionInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> ThisSenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), EachStimulus);

		if (ThisSenseClass == SenseToCheck)
		{
			OutSensedClass = ThisSenseClass;
			return EachStimulus.WasSuccessfullySensed();
		}
	}

	return false;
}

void AEnemyAIController::HandleSensedSight(AActor* Actor)
{
	if (!BB || !Actor) return;

	EEnemyState CurrentState = GetCurrentState();

	if (CurrentState == EEnemyState::Passive || CurrentState == EEnemyState::Investigating)
	{
		if (Actor == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			SetStateAsAttacking(Actor);
		}
	}
}

void AEnemyAIController::HandleSensedSound(const FVector& Location)
{
	if (!BB) return;

	EEnemyState CurrentState = GetCurrentState();

	if (CurrentState == EEnemyState::Passive)
	{
		SetStateAsInvestigating(Location);
	}
}

void AEnemyAIController::HandleSensedDamage(AActor* Actor)
{
	if (!BB || !Actor) return;

	EEnemyState CurrentState = GetCurrentState();

	if (CurrentState != EEnemyState::Attacking)
	{
		if (Actor == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			SetStateAsAttacking(Actor);
		}
	}
}

void AEnemyAIController::SetStateAsPassive()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Passive));
	}
}

void AEnemyAIController::SetStateAsAttacking(AActor* AttackTarget)
{
	if (BB)
	{
		if (AttackTarget)
		{
			BB->SetValueAsObject(TargetActorKey, AttackTarget);
		}

		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Attacking));
		TargetActor = AttackTarget;
	}
}

void AEnemyAIController::SetStateAsInvestigating(const FVector& Location)
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Investigating));
		BB->SetValueAsVector(InterestKey, Location);
	}
}

void AEnemyAIController::SetStateAsDead()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Dead));
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
