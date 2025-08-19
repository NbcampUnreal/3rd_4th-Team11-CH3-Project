// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_DefaultAttack.h"
#include "AI/EnemyActionInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

UBTT_DefaultAttack::UBTT_DefaultAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = TEXT("BTT Default Attack");
	bNotifyTick = false;
}

uint16 UBTT_DefaultAttack::GetInstanceMemorySize() const
{
	return sizeof(FAttackTaskMemory);
}

bool UBTT_DefaultAttack::IsTargetReachable(AAIController* AIController, AActor* Target) const
{
	if (!AIController || !Target) return false;
	UWorld* World = AIController->GetWorld();
	if (!World) return false;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
	if (!NavSys) return false;
	const FVector Start = AIController->GetPawn() ? AIController->GetPawn()->GetActorLocation() : AIController->GetNavAgentLocation();
	UNavigationPath* Path = NavSys->FindPathToActorSynchronously(World, Start, Target);

	if (!Path || !Path->IsValid() || Path->PathPoints.Num() < 2)
	{
		return false;
	}
	if (bTreatPartialPathAsUnreachable && Path->IsPartial())
	{
		return false;
	}

	return true;
}

bool UBTT_DefaultAttack::IsAttackFeasible(AAIController* AIController, AActor* Target) const
{
	if (!AIController || !Target) return false;
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;

	const FVector MyLoc = Pawn->GetActorLocation();
	const FVector TLoc = Target->GetActorLocation();
	const float HorizontalDist = FVector::Dist2D(MyLoc, TLoc);
	const float VerticalDelta = FMath::Abs(MyLoc.Z - TLoc.Z);

	if (HorizontalDist > AttackMaxRange) return false;
	if (VerticalDelta > MaxVerticalDelta) return false;

	if (bRequireLOS && !AIController->LineOfSightTo(Target))
	{
		return false;
	}
	return true;
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FAttackTaskMemory* Mem = (FAttackTaskMemory*)NodeMemory;
	Mem->Elapsed = 0.f;
	Mem->bStarted = false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(AIController->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	const bool bReachable = IsTargetReachable(AIController, TargetActor); // Check if it's reachable
	if (!bReachable)
	{
		if (!(bAllowAttackWithoutPath && IsAttackFeasible(AIController, TargetActor))) // Check if it's feasible
		{
			return EBTNodeResult::Failed;
		}
	}

	CachedOwnerComp = &OwnerComp;

	if (!Enemy->OnAttackEnd.IsAlreadyBound(this, &UBTT_DefaultAttack::OnAttackFinished)) // Check Delegate
	{
		Enemy->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
	}

	if (Enemy->GetClass()->ImplementsInterface(UEnemyActionInterface::StaticClass()))
	{
		IEnemyActionInterface::Execute_Attack(Enemy, TargetActor); // Execute Attack 
		Mem->bStarted = true;

		if (bFailIfMontageNotPlaying) // check if the montage is playing 
		{
			bool bPlaying = false;
			if (USkeletalMeshComponent* Mesh = Enemy->GetMesh())
			{
				if (UAnimInstance* Anim = Mesh->GetAnimInstance())
				{
					bPlaying = Anim->IsAnyMontagePlaying();
				}
			}
			if (!bPlaying)
			{
				Enemy->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
				CachedOwnerComp = nullptr;
				return EBTNodeResult::Failed;
			}
		}

		UWorld* World = AIController->GetWorld(); // Recheck using Timer (RecheckInterval)
		if (World)
		{
			FTimerDelegate Del;
			Del.BindUObject(this, &UBTT_DefaultAttack::Recheck, TWeakObjectPtr<UBehaviorTreeComponent>(&OwnerComp));
			World->GetTimerManager().SetTimer(Mem->RecheckTimerHandle, Del, RecheckInterval, true);
		}

		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTT_DefaultAttack::Recheck(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeak)
{
	if (!OwnerCompWeak.IsValid())
	{
		return;
	}
	UBehaviorTreeComponent& OwnerComp = *OwnerCompWeak.Get();


	FAttackTaskMemory* Mem = (FAttackTaskMemory*)OwnerComp.GetNodeMemory(this, OwnerComp.FindInstanceContainingNode(this));
	if (!Mem) return;
	Mem->Elapsed += RecheckInterval;

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	ABaseEnemy* Enemy = AIController ? Cast<ABaseEnemy>(AIController->GetPawn()) : nullptr;
	AActor* TargetActor = BBComp ? Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;

	if (!AIController || !TargetActor)
	{
		if (UWorld* W = OwnerComp.GetWorld()) W->GetTimerManager().ClearTimer(Mem->RecheckTimerHandle);
		if (bBroadcastEndOnAbort && Enemy) Enemy->CallOnAttackEnd();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Mem->bStarted && Mem->Elapsed >= AttackTimeout) // Timeout
	{
		if (UWorld* W = OwnerComp.GetWorld()) W->GetTimerManager().ClearTimer(Mem->RecheckTimerHandle);
		if (bBroadcastEndOnAbort && Enemy) Enemy->CallOnAttackEnd();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const bool bReachable = IsTargetReachable(AIController, TargetActor); // No Path, But Can Attack
	const bool bFeasible = IsAttackFeasible(AIController, TargetActor);
	if (!bReachable && !(bAllowAttackWithoutPath && bFeasible))
	{
		if (UWorld* W = OwnerComp.GetWorld()) W->GetTimerManager().ClearTimer(Mem->RecheckTimerHandle);
		if (bBroadcastEndOnAbort && Enemy) Enemy->CallOnAttackEnd();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

void UBTT_DefaultAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FAttackTaskMemory* Mem = (FAttackTaskMemory*)NodeMemory;

	if (AAIController* AI = OwnerComp.GetAIOwner()) // Clear Timer
	{
		if (UWorld* W = AI->GetWorld())
		{
			W->GetTimerManager().ClearTimer(Mem->RecheckTimerHandle);
		}

		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(AI->GetPawn())) // Clear Delegate
		{
			Enemy->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
			if (TaskResult != EBTNodeResult::Succeeded && bBroadcastEndOnAbort)
			{
				Enemy->CallOnAttackEnd();
			}
		}
	}
	CachedOwnerComp = nullptr;
}

void UBTT_DefaultAttack::OnAttackFinished()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}