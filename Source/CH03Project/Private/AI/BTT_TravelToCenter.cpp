// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTT_TravelToCenter.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Actor.h"

static FORCEINLINE bool IsMontagePlaying(ACharacter* Char, UAnimMontage* Montage)
{
	if (!Char || !Montage) return false;
	if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
	{
		return Anim->Montage_IsPlaying(Montage);
	}
	return false;
}

UBTT_TravelToCenter::UBTT_TravelToCenter()
{
	NodeName = TEXT("Travel To Center (Ready + Jump + Travel + Fall + Land)");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_TravelToCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;
	ACharacter* Boss = Cast<ACharacter>(AI->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	FState& St = *S(NodeMemory);
	St = FState{};
	St.Boss = Boss;
	St.AI = AI;

	FVector Center = FVector::ZeroVector;
	bool bHasCenter = false;

	if (BB->IsVectorValueSet(CenterKey.SelectedKeyName))
	{
		Center = BB->GetValueAsVector(CenterKey.SelectedKeyName);
		bHasCenter = true;
	}
	else if (AActor* CenterActor = Cast<AActor>(BB->GetValueAsObject(CenterKey.SelectedKeyName)))
	{
		Center = CenterActor->GetActorLocation();
		bHasCenter = true;
	}

	if (!bHasCenter)
	{
		Center = Boss->GetActorLocation();
	}

	St.TargetLocation = Center;

	EnterReady(St);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_TravelToCenter::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FState& St = *S(NodeMemory);
	if (St.AI.IsValid())
	{
		St.AI->StopMovement();
	}
	return EBTNodeResult::Aborted;
}

void UBTT_TravelToCenter::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FState& St = *S(NodeMemory);
	St.T += DeltaSeconds;

	ACharacter* Boss = St.Boss.Get();
	if (!Boss) { FinishClean(OwnerComp, St, false); return; }

	switch (St.Phase)
	{
	case EPhase::Ready:
	{
		if (!IsPlaying(Boss, Montage_Ready))
		{
			EnterJump(St);
		}
		break;
	}
	case EPhase::Jump:
	{
		if (!IsPlaying(Boss, Montage_Jump))
		{
			EnterTravel(St);
		}
		break;
	}
	case EPhase::Travel:
	{
		const float Dist2D = FVector::Dist2D(Boss->GetActorLocation(), St.TargetLocation);
		bool bArrived = (Dist2D <= AcceptRadius);

		if (St.AI.IsValid())
		{
			if (UPathFollowingComponent* PF = St.AI->GetPathFollowingComponent())
			{
				if (PF->GetStatus() == EPathFollowingStatus::Idle)
				{
					bArrived = bArrived || (Dist2D <= AcceptRadius * 2.f);
				}
			}
		}

		if (bArrived || St.T >= TravelTime)
		{
			EnterFall(St);
		}
		break;
	}
	case EPhase::Fall:
	{
		if (!IsPlaying(Boss, Montage_Fall))
		{
			EnterLand(St);
		}
		break;
	}
	case EPhase::Land:
	{
		if (!IsPlaying(Boss, Montage_Land))
		{
			FinishClean(OwnerComp, St, true);
		}
		break;
	}
	default:
		FinishClean(OwnerComp, St, true);
		break;
	}
}

void UBTT_TravelToCenter::EnterReady(FState& St) const
{
	St.Phase = EPhase::Ready;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (Montage_Ready)
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				Anim->Montage_Play(Montage_Ready, 1.f);
			}
		}
	}
}

void UBTT_TravelToCenter::EnterJump(FState& St) const
{
	St.Phase = EPhase::Jump;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (Montage_Jump)
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				Anim->Montage_Play(Montage_Jump, 1.f);
			}
		}
	}
}

void UBTT_TravelToCenter::EnterTravel(FState& St) const
{
	St.Phase = EPhase::Travel;
	St.T = 0.f;

	if (St.AI.IsValid())
	{
		FAIMoveRequest Req(St.TargetLocation);
		Req.SetAcceptanceRadius(AcceptRadius);
		Req.SetUsePathfinding(true);
		St.MoveId = St.AI->MoveTo(Req);
	}

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (Montage_Travel)
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				Anim->Montage_Play(Montage_Travel, 1.f);
			}
		}
	}
}

void UBTT_TravelToCenter::EnterFall(FState& St) const
{
	St.Phase = EPhase::Fall;
	St.T = 0.f;

	if (St.AI.IsValid())
	{
		St.AI->StopMovement();
	}

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (Montage_Fall)
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				Anim->Montage_Play(Montage_Fall, 1.f);
			}
		}
	}
}

void UBTT_TravelToCenter::EnterLand(FState& St) const
{
	St.Phase = EPhase::Land;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (Montage_Land)
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				Anim->Montage_Play(Montage_Land, 1.f);
			}
		}
	}
}

void UBTT_TravelToCenter::FinishClean(UBehaviorTreeComponent& OwnerComp, FState& St, bool bSuccess)
{
	if (St.AI.IsValid())
	{
		St.AI->StopMovement();
	}
	FinishLatentTask(OwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

bool UBTT_TravelToCenter::IsPlaying(ACharacter* Char, UAnimMontage* Montage)
{
	return IsMontagePlaying(Char, Montage);
}
