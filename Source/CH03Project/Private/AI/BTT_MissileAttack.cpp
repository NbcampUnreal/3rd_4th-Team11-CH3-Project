// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MissileAttack.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

#include "BaseStatComponent.h"

UBTT_MissileAttack::UBTT_MissileAttack()
{
	NodeName = TEXT("Missile Attack (Ready + Fire + Fall + Land)");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MissileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;
	ACharacter * Char = Cast<ACharacter>(AICon->GetPawn());
	if (!Char) return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;


	BTWeak = &OwnerComp;
	AIWeak = AICon;

	FState& St = *S(NodeMemory);
	St = FState{};
	St.Boss = Char;
	St.AI = AICon;
	if (BB->GetValueAsInt(NumPointsKey.SelectedKeyName) > 0)
	{
		St.NumCircles = BB->GetValueAsInt(NumPointsKey.SelectedKeyName);
		NumCircles = St.NumCircles;
	}

	EnterReady(OwnerComp, St);


	if (Query)
	{
		if (UEnvQueryInstanceBlueprintWrapper* Wrapper = UEnvQueryManager::RunEQSQuery(Char->GetWorld(), Query, AICon, EEnvQueryRunMode::AllMatching, UEnvQueryInstanceBlueprintWrapper::StaticClass()))
		{
			ActiveQuery = Wrapper;
			Wrapper->GetOnQueryFinishedEvent().AddDynamic(this, &UBTT_MissileAttack::OnQueryFinished);
		}
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_MissileAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FState& St = *S(NodeMemory);
	DestroyDecals(St);
	return EBTNodeResult::Aborted;
}

void UBTT_MissileAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type)
{
	FState& St = *S(NodeMemory);
	DestroyDecals(St);
}

void UBTT_MissileAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FState& St = *S(NodeMemory);
	St.T += DeltaSeconds;

	UWorld* World = OwnerComp.GetWorld();
	if (!World) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	switch (St.Phase)
	{
	case EPhase::Ready:
	{
		if (ACharacter* Boss = St.Boss.Get())
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				if (!Montage_Ready || !Anim->Montage_IsPlaying(Montage_Ready))
				{
					EnterFire(OwnerComp, St);
				}
			}
			else
			{
				EnterFire(OwnerComp, St);
			}
		}
		break;
	}

	case EPhase::Fire:
	{
		if (!St.bDecalsPlaced && St.T >= DelayBeforeDecals && St.LockedImpacts.Num() > 0)
		{
			SpawnDecals(St, true);
			St.bDecalsPlaced = true;
		}

		if (!St.bExploded && St.T >= (DelayBeforeDecals + ExplosionDelay))
		{
			DoImpact(St);
			St.bExploded = true;
			if (bDestroyDecalOnFire)
			{
				DestroyDecals(St);
			}

			EnterFalling(OwnerComp, St);
			break;
		}

		if (ACharacter* Boss = St.Boss.Get())
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				if (Montage_Fire && !Anim->Montage_IsPlaying(Montage_Fire) && St.bExploded)
				{
					EnterFalling(OwnerComp, St);
				}
			}
		}
		break;
	}

	case EPhase::Falling:
	{
		if (ACharacter* Boss = St.Boss.Get())
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				if (!Montage_Fall || !Anim->Montage_IsPlaying(Montage_Fall))
				{
					EnterLanding(OwnerComp, St);
				}
			}
			else
			{
				EnterLanding(OwnerComp, St);
			}
		}
		break;
	}

	case EPhase::Landing:
	{
		if (ACharacter* Boss = St.Boss.Get())
		{
			if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				if (!Montage_Land || !Anim->Montage_IsPlaying(Montage_Land))
				{
					EnterFinish(OwnerComp, St, true);
				}
			}
			else
			{
				EnterFinish(OwnerComp, St, true);
			}
		}
		break;
	}

	default:
		EnterFinish(OwnerComp, St, true);
		break;
	}
}

void UBTT_MissileAttack::EnterReady(UBehaviorTreeComponent& BTC, FState& St) const
{
	St.Phase = EPhase::Ready;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (Montage_Ready) Anim->Montage_Play(Montage_Ready, 1.f);
		}
	}
}

void UBTT_MissileAttack::EnterFire(UBehaviorTreeComponent& BTC, FState& St) const
{
	St.Phase = EPhase::Fire;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (Montage_Fire) Anim->Montage_Play(Montage_Fire, 1.f);
		}
	}
}

void UBTT_MissileAttack::EnterFalling(UBehaviorTreeComponent& BTC, FState& St) const
{
	St.Phase = EPhase::Falling;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (Montage_Fall) Anim->Montage_Play(Montage_Fall, 1.f);
		}
	}
}

void UBTT_MissileAttack::EnterLanding(UBehaviorTreeComponent& BTC, FState& St) const
{
	St.Phase = EPhase::Landing;
	St.T = 0.f;

	if (ACharacter* Boss = St.Boss.Get())
	{
		if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
		{
			if (Montage_Land) Anim->Montage_Play(Montage_Land, 1.f);
		}
	}
}

void UBTT_MissileAttack::EnterFinish(UBehaviorTreeComponent& BTC, FState& St, bool bSuccess) const
{
	DestroyDecals(St);
	FinishLatentTask(BTC, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTT_MissileAttack::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* Wrapper, EEnvQueryStatus::Type Status)
{
	if (Status != EEnvQueryStatus::Success || !BTWeak.IsValid()) return;

	TArray<FVector> Raw;
	Wrapper->GetQueryResultsAsLocations(Raw);

	FRandomStream RNG(FPlatformTime::Cycles());
	RandomShuffle(Raw, RNG);
	if (Raw.Num() > NumCircles)
	{
		Raw.SetNum(NumCircles, true);
	}

	UWorld* World = AIWeak.IsValid() ? AIWeak->GetWorld() : nullptr;
	TArray<FVector> Final; Final.Reserve(Raw.Num());
	for (const FVector& P : Raw)
	{
		FVector G = P;
		if (World) ProjectToGround(World, P, G, GroundTraceChannel, DecalZOffset);
		Final.Add(G);
	}

	if (UBehaviorTreeComponent* BTC = BTWeak.Get())
	{
		const int32 InstIdx = BTC->FindInstanceContainingNode(this);
		if (InstIdx != INDEX_NONE)
		{
			if (uint8* NodeMemory = BTC->GetNodeMemory(this, InstIdx))
			{
				FState& St = *S(NodeMemory);
				St.LockedImpacts = MoveTemp(Final);
			}
		}
	}
}

bool UBTT_MissileAttack::ProjectToGround (UWorld* World, const FVector& In, FVector& Out, TEnumAsByte<ECollisionChannel> Channel, float ZOffset)
{
	if (!World) return false;

	const FVector Start = In + FVector(0, 0, 800.f);
	const FVector End = In - FVector(0, 0, 2000.f);

	FHitResult Hit;
	FCollisionQueryParams P(SCENE_QUERY_STAT(Missile_GroundTrace), false);
	if (World->LineTraceSingleByChannel(Hit, Start, End, Channel, P))
	{
		Out = Hit.ImpactPoint + FVector(0, 0, ZOffset);
		return true;
	}
	return false;
}

void UBTT_MissileAttack::SpawnDecals(FState& St, bool bLock) const
{
	if (!DecalMaterial) return;
	if (!St.Boss.IsValid()) return;

	UWorld* World = St.Boss->GetWorld();
	if (!World) return;

	for (const FVector& L : St.LockedImpacts)
	{
		const FVector Size(DecalThickness, DecalRadius, DecalRadius);
		const FRotator Rot(-90.f, 0.f, 0.f);

		const float Life = FMath::Max(ExplosionDelay + 0.5f, 0.5f);

		UDecalComponent* D = UGameplayStatics::SpawnDecalAtLocation(World, DecalMaterial, Size, L, Rot, Life);

		if (D)
		{
			D->SetFadeScreenSize(0.f);
			D->SetSortOrder(10);
			St.Decals.Add(D);
		}
	}
}

void UBTT_MissileAttack::DestroyDecals(FState& St) const
{
	for (auto& W : St.Decals)
	{
		if (UDecalComponent* D = W.Get())
		{
			if (AActor* Owner = D->GetOwner())
			{
				Owner->Destroy();
			}
			else
			{
				D->DestroyComponent();
			}
		}
	}
	St.Decals.Reset();
}


void UBTT_MissileAttack::DoImpact(FState& St) const
{
	if (!St.Boss.IsValid()) return;
	UWorld* World = St.Boss->GetWorld();
	if (!World) return;

	for (const FVector& Center : St.LockedImpacts)
	{
		if (ImpactFX)
		{
			const float Scale = (DecalRadius / 100.f) * ImpactFXScaleAtRadius100;
			UGameplayStatics::SpawnEmitterAtLocation(World, ImpactFX, Center, FRotator::ZeroRotator, FVector(Scale), true);
		}

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
		ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> Ignore;
		if (St.Boss.IsValid()) Ignore.Add(St.Boss.Get());

		TArray<AActor*> Hits;
		if (UKismetSystemLibrary::SphereOverlapActors(World, Center, DecalRadius, ObjTypes, nullptr, Ignore, Hits))
		{
			for (AActor* A : Hits)
			{
				if (!A) continue;
				if (!A->ActorHasTag(PlayerTag)) continue;

				float CapsuleRadius = 0.f;
				if (const UCapsuleComponent* Cap = A->FindComponentByClass<UCapsuleComponent>())
				{
					CapsuleRadius = Cap->GetScaledCapsuleRadius();
				}

				const float Dist2D = FVector::Dist2D(A->GetActorLocation(), Center);
				if (Dist2D <= DecalRadius + CapsuleRadius)
				{
					if (UBaseStatComponent* Stat = A->FindComponentByClass<UBaseStatComponent>())
					{
						//UE_LOG(LogTemp, Verbose, TEXT("MissileAttack hit %s"), *A->GetName());
						Stat->AddHp(-Damage);
					}
				}
			}
		}
	}
}
