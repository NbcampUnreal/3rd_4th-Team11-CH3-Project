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

#include "Engine/Engine.h"

#define DEBUG_MSG(Key, Time, Color, Format, ...) \
if (GEngine) GEngine->AddOnScreenDebugMessage(Key, Time, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__))


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	BB = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BT = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
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
		DEBUG_MSG(-1, 3.f, FColor::Green, "[AIController] Perception bound successfully.");
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
			DEBUG_MSG(-1, 3.f, FColor::Cyan, "[AIController] Initial State: %d (Passive=0)", (uint8)GetCurrentState());
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

	DEBUG_MSG(-1, 2.f, FColor::Blue, "[Sight] Current State: %d", (uint8)GetCurrentState());

	BB->SetValueAsObject(TargetActorKey, Actor);
	SetStateAsAttacking();
	DEBUG_MSG(-1, 2.f, FColor::Green, "[Sight] TargetActor set to %s | State -> Attacking", *Actor->GetName());
}

void AEnemyAIController::HandleSensedSound(const FVector& Location)
{
	if (!BB) return;

	DEBUG_MSG(-1, 2.f, FColor::Orange, "[Hearing] Location: %s", *Location.ToString());
	BB->SetValueAsVector(InterestKey, Location);
	SetStateAsInvestigating(Location);
}

void AEnemyAIController::HandleSensedDamage(AActor* Actor)
{
	if (!BB || !Actor) return;

	DEBUG_MSG(-1, 2.f, FColor::Magenta, "[Damage] Current State: %d", (uint8)GetCurrentState());

	if (GetCurrentState() == EEnemyState::Attacking)
	{
		DEBUG_MSG(-1, 2.f, FColor::Red, "[Damage] Already Attacking - Ignored");
		return;
	}

	BB->SetValueAsObject(TargetActorKey, Actor);
	SetStateAsAttacking();
	DEBUG_MSG(-1, 2.f, FColor::Green, "[Damage] TargetActor set to %s | State -> Attacking", *Actor->GetName());
}

void AEnemyAIController::SetStateAsPassive()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Passive));
		DEBUG_MSG(-1, 1.5f, FColor::Cyan, "[State] -> Passive");
	}
}

void AEnemyAIController::SetStateAsAttacking()
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Attacking));
		DEBUG_MSG(-1, 1.5f, FColor::Green, "[State] -> Attacking");
	}
}

void AEnemyAIController::SetStateAsInvestigating(const FVector& Location)
{
	if (BB)
	{
		BB->SetValueAsEnum(StateKey, static_cast<uint8>(EEnemyState::Investigating));
		BB->SetValueAsVector(InterestKey, Location);
		DEBUG_MSG(-1, 1.5f, FColor::Yellow, "[State] -> Investigating");
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
