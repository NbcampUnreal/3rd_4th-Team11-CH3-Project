// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTT_LRMAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimInstance.h" 
#include "Engine/World.h"

UBTT_LRMAttack::UBTT_LRMAttack()
{
	NodeName = TEXT("LRM Jump Fire (Ready + Tracking + Fire + Fall + Land)");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_LRMAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;
	ACharacter* Char = Cast<ACharacter>(AICon->GetPawn());
	if (!Char) return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	FState& St = *S(NodeMemory);
	St = FState{};
	St.AI = AICon;
	St.Boss = St.AI.IsValid() ? Char : nullptr;

	UAnimInstance* Anim = St.Boss->GetMesh() ? St.Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!Anim || !Montage_Ready || !Montage_JumpFire || !Montage_JumpFall || !Montage_JumpLand)
		return EBTNodeResult::Failed;

	EnterReady(St);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_LRMAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FState& St = *S(NodeMemory);
	if (St.Boss.IsValid() && St.Boss->GetMesh())
	{
		if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
		{
			Anim->StopAllMontages(0.f);
		}
	}
	DestroyDecal(St);
	return EBTNodeResult::Aborted;
}

void UBTT_LRMAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type /*Result*/)
{
	FState& St = *S(NodeMemory);
	DestroyDecal(St);
}

void UBTT_LRMAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FState& St = *S(NodeMemory);
	if (!St.Boss.IsValid()) { FinishClean(OwnerComp, St, false); return; }

	St.T += DeltaSeconds;

	switch (St.Phase)
	{
	case EPhase::Ready:
	{
		if (bFollowTargetWhileTracking)
		{
			UpdateTrackedGround(St);
			SpawnOrUpdateDecal(St, false);
		}

		if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
		{
			if (!Anim->Montage_IsPlaying(Montage_Ready))
			{
				EnterTracking(St);
			}
		}
	}
	break;

	case EPhase::Tracking:
	{
		if (!St.bImpactLocked)
		{
			UpdateTrackedGround(St);
			SpawnOrUpdateDecal(St, false);

			if (St.T >= St.MissileTravelTime - 1.0f)
			{
				St.LockedImpact = St.TrackedGround;
				St.bImpactLocked = true;
				SpawnOrUpdateDecal(St, true);
			}
		}
		else
		{
			SpawnOrUpdateDecal(St, true);
		}

		if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
		{
			if (!Anim->Montage_IsPlaying(Montage_JumpFire))
			{
				if (!St.bImpactLocked)
				{
					St.LockedImpact = St.TrackedGround;
					St.bImpactLocked = true;
					SpawnOrUpdateDecal(St, true);
				}

				DoImpact(St);
				EnterFalling(St);
			}
		}
	}
	break;

	case EPhase::Falling:
	{
		if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
		{
			if (!Anim->Montage_IsPlaying(Montage_JumpFall))
			{
				EnterLanding(St);
			}
		}
	}
	break;

	case EPhase::Landing:
	{
		if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
		{
			if (!Anim->Montage_IsPlaying(Montage_JumpLand))
			{
				FinishClean(OwnerComp, St, true);
			}
		}
	}
	break;

	default: break;
	}
}

void UBTT_LRMAttack::EnterReady(FState& St) const
{
	if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(Montage_Ready, 1.f);
	}

	UpdateTrackedGround(St);
	SpawnOrUpdateDecal(St, false);

	St.T = 0.f;
	St.Phase = EPhase::Ready;
}

void UBTT_LRMAttack::EnterTracking(FState& St) const
{
	UpdateTrackedGround(St);
	SpawnOrUpdateDecal(St, false);

	if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(Montage_JumpFire, 1.f);
	}

	St.T = 0.f;
	St.Phase = EPhase::Tracking;
}

void UBTT_LRMAttack::EnterFalling(FState& St) const
{
	if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(Montage_JumpFall, 1.f);
	}
	St.T = 0.f;
	St.Phase = EPhase::Falling;
}

void UBTT_LRMAttack::EnterLanding(FState& St) const
{
	if (UAnimInstance* Anim = St.Boss->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(Montage_JumpLand, 1.f);
	}
	St.T = 0.f;
	St.Phase = EPhase::Landing;
}

void UBTT_LRMAttack::FinishClean(UBehaviorTreeComponent& OwnerComp, FState& St, bool bSuccess)
{
	DestroyDecal(St);
	FinishLatentTask(OwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

bool UBTT_LRMAttack::UpdateTrackedGround(FState& St) const
{
	if (!St.AI.IsValid()) return false;

	UBlackboardComponent* BB = St.AI->GetBlackboardComponent();
	AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;
	if (!Target) return false;

	UWorld* World = St.Boss->GetWorld();
	const FVector Start = Target->GetActorLocation() + FVector(0, 0, 800.f);
	const FVector End = Target->GetActorLocation() - FVector(0, 0, 2000.f);

	FHitResult Hit;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(LRM_Ground), false, St.Boss.Get());
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(St.Boss.Get());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Destructible);

	if (World->LineTraceSingleByObjectType(Hit, Start, End, ObjParams, Params))
	{
		St.TrackedGround = Hit.ImpactPoint + FVector(0, 0, DecalZOffset);
		return true;
	}

	if (World->LineTraceSingleByChannel(Hit, Start, End, GroundTraceChannel, Params))
	{
		if (Hit.GetActor() && Hit.GetActor() != Target && Hit.GetActor() != St.Boss.Get())
		{
			St.TrackedGround = Hit.ImpactPoint + FVector(0, 0, DecalZOffset);
			return true;
		}
	}
	return false;
}

void UBTT_LRMAttack::SpawnOrUpdateDecal(FState& St, bool bLock) const
{
	if (!DecalMaterial) return;

	if (!St.Decal.IsValid())
	{
		UWorld* World = St.Boss->GetWorld();
		UDecalComponent* Decal = NewObject<UDecalComponent>(World->GetWorldSettings());
		Decal->RegisterComponent();

		Decal->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
		Decal->SetFadeScreenSize(0.f);
		Decal->SetDecalMaterial(DecalMaterial);
		// Decal->SetFadeOut(LifeTime, 0.1f, false);

		St.Decal = Decal;
	}

	const float D = DecalRadius * 2.f;
	St.Decal->DecalSize = FVector(DecalThickness, D, D);
	St.Decal->SetWorldLocation(bLock ? St.LockedImpact : St.TrackedGround);
}


void UBTT_LRMAttack::DestroyDecal(FState& St) const
{
	if (St.Decal.IsValid())
	{
		St.Decal->DestroyComponent();
		St.Decal.Reset();
		St.DecalMID.Reset();
	}
}

void UBTT_LRMAttack::DoImpact(FState& St) const
{
	UWorld* World = St.Boss.IsValid() ? St.Boss->GetWorld() : nullptr;
	if (!World) return;

	if (ImpactFX)
	{
		const float S = (DecalRadius / 100.f) * ImpactFXScaleAtRadius100;
		UGameplayStatics::SpawnEmitterAtLocation(World, ImpactFX, St.LockedImpact, FRotator::ZeroRotator, FVector(S));
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;
	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> Ignore;
	if (St.Boss.IsValid()) Ignore.Add(St.Boss.Get());

	TArray<AActor*> Hits;
	const FVector Center = St.LockedImpact;

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
					//UE_LOG(LogTemp, Warning, TEXT("LRM Attack hit %s"), *A->GetName());
					Stat->AddHp(-Damage);
				}
			}
		}
	}
}
