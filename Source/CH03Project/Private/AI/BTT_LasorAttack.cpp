// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTT_LasorAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "BaseStatComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogLaserAttack, Log, All);

UBTT_LasorAttack::UBTT_LasorAttack()
{
	NodeName = TEXT("Laser Attack (Turn L or R)");
	bNotifyTick = true;
	DamageAmountPS = 60.f;
}

EBTNodeResult::Type UBTT_LasorAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BTWeak = &OwnerComp;

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;
	ACharacter* Boss = Cast<ACharacter>(AI->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActoryKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	FState& St = *S(NodeMemory);
	St = FState{};
	St.Boss = Boss;
	St.AI = AI;

	const FTransform Mx = Boss->GetMesh()->GetSocketTransform(MuzzleSocket);
	const FVector Src = Mx.GetLocation();
	FVector Tgt = Target->GetActorLocation();
	Tgt.Z = Src.Z; 

	const float YawToTgt = (Tgt - Src).GetSafeNormal().Rotation().Yaw;
	St.StartYaw = Boss->GetActorRotation().Yaw;
	St.TargetYawOffset = FMath::FindDeltaAngleDegrees(St.StartYaw, YawToTgt);

	St.bTurnRight = (St.TargetYawOffset >= 0.f);
	St.BeamLen = 0.f;

	EnterReady(St);
	EnterTurn(St);
	return EBTNodeResult::InProgress;
}

void UBTT_LasorAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FState& St = *S(NodeMemory);
	if (!St.Boss.IsValid()) 
	{ 
		EnterFinish(OwnerComp, St, false); 
		return; 
	}

	St.T += DeltaSeconds;

	switch (St.Phase)
	{
	case EPhase::Turn:
	{
		FRotator R = St.Boss->GetActorRotation();
		const float Sign = St.bTurnRight ? +1.f : -1.f;
		R.Yaw += AngularSpeedDeg * Sign * DeltaSeconds;
		St.Boss->SetActorRotation(R);

		UpdateLaserAndDamage(St, DeltaSeconds);

		if (St.T >= Duration)
		{
			EnterFinish(OwnerComp, St, true);
		}
		break;
	}
	default:
		EnterFinish(OwnerComp, St, true);
		break;
	}
}

EBTNodeResult::Type UBTT_LasorAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FState& St = *S(NodeMemory);
	StopLaser(St);

	if (St.Boss.IsValid())
	{
		if (UAnimInstance* Anim = St.Boss->GetMesh() ? St.Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (Montage_lft) Anim->Montage_Stop(0.1f, Montage_lft);
			if (Montage_rt)  Anim->Montage_Stop(0.1f, Montage_rt);
		}
	}
	return EBTNodeResult::Aborted;
}

void UBTT_LasorAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type)
{
	FState& St = *S(NodeMemory);
	StopLaser(St);
}

void UBTT_LasorAttack::EnterReady(FState& St) const
{
	if (ACharacter* Boss = St.Boss.Get())
	{
		if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (St.bTurnRight && Montage_rt)        Anim->Montage_Play(Montage_rt, 1.f);
			else if (!St.bTurnRight && Montage_lft) Anim->Montage_Play(Montage_lft, 1.f);
		}
	}
}

void UBTT_LasorAttack::EnterTurn(FState& St) const
{
	St.Phase = EPhase::Turn;
	St.T = 0.f;
	StartLaser(St);
}

void UBTT_LasorAttack::EnterFinish(UBehaviorTreeComponent& BTC, FState& St, bool bSuccess) const
{
	StopLaser(St);
	FinishLatentTask(BTC, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTT_LasorAttack::StartLaser(FState& St) const
{
	if (!LaserSystem || !St.Boss.IsValid()) return;
	USkeletalMeshComponent* Mesh = St.Boss->GetMesh();
	if (!Mesh) return;
	UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(LaserSystem, Mesh, MuzzleSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false, true);
	if (!NC) return;

	NC->SetTickGroup(TG_PostUpdateWork);
	NC->AddTickPrerequisiteComponent(Mesh);
	const FTransform Sx = Mesh->GetSocketTransform(MuzzleSocket);
	NC->SetWorldTransform(Sx);

	St.BeamLen = 0.f;

	const FVector SrcWS = Sx.GetLocation();
	const float   AimYaw = St.Boss->GetActorRotation().Yaw + St.TargetYawOffset;
	const FVector AimDir = FRotationMatrix(FRotator(0.f, AimYaw, 0.f)).GetUnitAxis(EAxis::X);
	const FVector EndWS = SrcWS + AimDir * 10.f;
	const FVector EndL = NC->GetComponentTransform().InverseTransformPosition(EndWS);

	SetNiagaraVec3(NC, BeamStartParam, FVector::ZeroVector);
	SetNiagaraVec3(NC, BeamEndParam, EndL);

	St.LaserNS = NC;
}


void UBTT_LasorAttack::StopLaser(FState& St) const
{
	if (UNiagaraComponent* NC = St.LaserNS.Get())
	{
		NC->Deactivate();
		NC->DestroyComponent();
	}
	St.LaserNS.Reset();
}

void UBTT_LasorAttack::UpdateLaserAndDamage(FState& St, float DeltaSeconds) const
{
	if (!St.Boss.IsValid()) return;
	USkeletalMeshComponent* Mesh = St.Boss->GetMesh();
	UWorld* World = St.Boss->GetWorld();
	if (!Mesh || !World) return;
	const FTransform Sx = Mesh->GetSocketTransform(MuzzleSocket);
	const FVector    SrcWS = Sx.GetLocation();
	if (UNiagaraComponent* NC = St.LaserNS.Get()) NC->SetWorldTransform(Sx);

	St.BeamLen = FMath::Min(LaserRange, St.BeamLen + BeamExtendSpeed * DeltaSeconds);
	const float   AimYaw = St.Boss->GetActorRotation().Yaw + St.TargetYawOffset;
	const FVector AimDir = FRotationMatrix(FRotator(0.f, AimYaw, 0.f)).GetUnitAxis(EAxis::X);
	const FVector EndWantedWS = SrcWS + AimDir * St.BeamLen;

	FHitResult Hit;
	FCollisionQueryParams P(SCENE_QUERY_STAT(LaserHit), false, St.Boss.Get());
	const bool   bHit = World->LineTraceSingleByChannel(Hit, SrcWS, EndWantedWS, TraceChannel, P);
	const FVector EndWS = bHit ? Hit.ImpactPoint : EndWantedWS;

	if (UNiagaraComponent* NC = St.LaserNS.Get())
	{
		const FVector EndL = NC->GetComponentTransform().InverseTransformPosition(EndWS);
		SetNiagaraVec3(NC, BeamStartParam, FVector::ZeroVector);
		SetNiagaraVec3(NC, BeamEndParam, EndL);
	}


	TArray<FHitResult> Hits;
	const ETraceTypeQuery TT = UEngineTypes::ConvertToTraceType(TraceChannel);
	if (UKismetSystemLibrary::SphereTraceMulti(World, SrcWS, EndWS, BeamDamageRadius, TT, false, {}, EDrawDebugTrace::ForDuration, Hits, true))
	{
		for (const FHitResult& H : Hits)
		{
			if (AActor* A = H.GetActor())
			{
				if (A->ActorHasTag(PlayerTag) && A != St.Boss.Get())
				{
					ApplyTransDamageOrFallback(A, DamageAmountPS);
				}
			}
		}
	}
}

void UBTT_LasorAttack::SetNiagaraVec3(UNiagaraComponent* NC, const FName& Param, const FVector& V)
{
	if (!NC) return;

#if ENGINE_MAJOR_VERSION >= 5
	NC->SetVariableVec3(Param, V);
#else
	NC->SetNiagaraVariableVec3(Param, V);
#endif
	const FString S = Param.ToString();
	if (!S.StartsWith(TEXT("User.")))
	{
		const FName UserName(*FString::Printf(TEXT("User.%s"), *S));
#if ENGINE_MAJOR_VERSION >= 5
		NC->SetVariableVec3(UserName, V);
#else
		NC->SetNiagaraVariableVec3(UserName, V);
#endif
	}
}

void UBTT_LasorAttack::ApplyTransDamageOrFallback(AActor* Target, float Damage)
{
	if (!Target) return;

	if (UBaseStatComponent* Stat = Target->FindComponentByClass<UBaseStatComponent>())
	{
		Stat->AddHp(-static_cast<int32>(Damage));
		UE_LOG(LogLaserAttack, Warning, TEXT("Laser Attack hit %s"), *Target->GetName());
	}
}
