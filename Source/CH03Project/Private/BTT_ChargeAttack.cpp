// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_ChargeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "BaseStatComponent.h"
#include "Engine/DecalActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	NodeName = TEXT("Charge Attack (Ready + Montages + Dash)");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();                 
	if (!AICon)  return EBTNodeResult::Failed;
	ACharacter* Char = Cast<ACharacter>(AICon->GetPawn());      
	if (!Char)   return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent(); 
	if (!BB)     return EBTNodeResult::Failed;
	UWorld* World = Char->GetWorld();                              
	if (!World)  return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;


	FState& St = *S(NodeMemory);
	St = FState{};
	St.SavedAI = AICon;
	St.SavedFocusActor = AICon->GetFocusActor();

	FVector Start = Char->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	FVector G;
	if (ProjectToGround(World, Start, G))      Start = G;
	if (ProjectToGround(World, TargetLoc, G))  TargetLoc = G;

	const FVector ToTarget = (TargetLoc - Start);
	const float FullDist = ToTarget.Size();
	if (FullDist < KINDA_SMALL_NUMBER) return EBTNodeResult::Failed;

	const float ClampedDist = FMath::Clamp(FullDist, MinChargeDistance, MaxChargeDistance);
	if (ClampedDist < KINDA_SMALL_NUMBER) return EBTNodeResult::Failed;

	const FVector Dir = ToTarget / FullDist;
	const FVector FinalDest = Start + Dir * ClampedDist;

	St.Start = Start;
	St.Dest = FinalDest;
	St.Dir = Dir;
	St.Distance = ClampedDist;

	BB->SetValueAsVector(ChargeDirectionBBKeyName, Dir);
	BB->SetValueAsVector(ChargeDestinationBBKeyName, FinalDest);

	St.Phase = EPhase::Ready;
	St.T = 0.f;
	EnterTelegraph(Char, St, BB);

	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AICon = OwnerComp.GetAIOwner(); 
	if (!AICon) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }
	ACharacter* Char = Cast<ACharacter>(AICon->GetPawn()); 
	if (!Char) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	FState& St = *S(NodeMemory);
	St.T += DeltaSeconds;

	switch (St.Phase)
	{
	case EPhase::Ready:
	{
		if (RecomputePathFromCurrentTarget(Char, OwnerComp.GetBlackboardComponent(), St))
		{
			UpdateTelegraphDecalTransform(St);
		}

		if (St.T >= St.PrechargeDur)
		{
			St.Phase = EPhase::JumpUp;
			St.T = 0.f;
			EnterJumpUp(Char, St);
		}

		break;
	}
	case EPhase::JumpUp:
	{
		if (!St.bPathLocked)
		{
			if (RecomputePathFromCurrentTarget(Char, OwnerComp.GetBlackboardComponent(), St))
			{
				UpdateTelegraphDecalTransform(St);
			}
		}

		if (St.T >= St.JumpUpDur)
		{
			St.Phase = EPhase::Dash; St.T = 0.f;
			EnterDash(Char, St);
		}
		break;
	}
	case EPhase::Dash:
	{
		if (auto* Move = Char->GetCharacterMovement())
		{
			if (Move->MovementMode == MOVE_Falling && St.T > 0.05f)
			{
				Move->SetMovementMode(MOVE_Walking);
			}
		}

		const float Traveled = FVector::Dist2D(Char->GetActorLocation(), St.DashStart);
		const bool  bTimeout = St.T > (St.Distance / FMath::Max(1.f, ChargeSpeed) + 0.4f);

		if (Traveled + KINDA_SMALL_NUMBER >= St.Distance)
		{
			FHitResult Hit;
			Char->SetActorLocation(St.Dest, true, &Hit);
			ForceStopCharacter(Char);
			St.Phase = EPhase::Finish; St.T = 0.f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}

		if (bTimeout)
		{
			ForceStopCharacter(Char);
			St.Phase = EPhase::Finish; St.T = 0.f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}

		if (Char->Tags.Contains(TEXT("ChargeHitOnce")) && St.bHitOnce && St.T >= GraceTimeAfterHit)
		{
			ForceStopCharacter(Char);
			St.Phase = EPhase::Finish; St.T = 0.f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		break;
	}
	case EPhase::Finish:
	default:
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		break;
	}
}

void UBTT_ChargeAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result)
{
	if (AAIController* AICon = OwnerComp.GetAIOwner())
	{
		if (ACharacter* Char = Cast<ACharacter>(AICon->GetPawn()))
		{
			FState& St = *S(NodeMemory);
			RestoreFocusAfterDash(AICon, OwnerComp.GetBlackboardComponent(), St);
			ExitAll(Char, St);
		}
	}
}

bool UBTT_ChargeAttack::ProjectToGround(UWorld* World, const FVector& InLoc, FVector& OutLoc) const
{
	if (!World) return false;
	FHitResult Hit;
	const FVector A = InLoc + FVector(0, 0, GroundProbeUp);
	const FVector B = InLoc - FVector(0, 0, GroundProbeDown);

	const bool bHit = UKismetSystemLibrary::LineTraceSingle(World, A, B, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {}, EDrawDebugTrace::None, Hit, true);

	if (bHit) { OutLoc = Hit.Location; return true; }
	return false;
}

FRotator UBTT_ChargeAttack::GroundAlignedYaw(const FVector& Dir) const
{
	return FRotator(-90.f, Dir.Rotation().Yaw + DecalYawOffset, 0.f);
}

UDecalComponent* UBTT_ChargeAttack::SpawnTelegraphDecal(UWorld* World, const FVector& Start, const FVector& End) const
{
	if (!World || !TelegraphDecalMaterial) return nullptr;

	const FVector V = End - Start;
	const float   Den = V.Size();
	if (Den < KINDA_SMALL_NUMBER) return nullptr;

	const FVector Dir = V / Den;
	const float   Len = MaxChargeDistance;
	const FVector Mid = Start + Dir * (Len * 0.5f);
	const FRotator Rot = GroundAlignedYaw(Dir);

	ADecalActor* DecalActor = World->SpawnActor<ADecalActor>(Mid, Rot);
	if (!DecalActor) return nullptr;

	UDecalComponent* Decal = DecalActor->GetDecal();
	Decal->DecalSize = FVector(DecalProjectionDepth, Len * 0.5f, DecalWidth * 0.5f);
	Decal->SetFadeOut(0.f, 0.f, false);
	DecalActor->SetLifeSpan(DecalLifeTime);
	DecalActor->SetActorScale3D(FVector::OneVector);
	Decal->SetDecalMaterial(TelegraphDecalMaterial);

	if (UMaterialInstanceDynamic* MID = Decal->CreateDynamicMaterialInstance())
	{
		MID->SetScalarParameterValue(TEXT("OpacityMult"), 1.f);
	}

	return Decal;
}

bool UBTT_ChargeAttack::RecomputePathFromCurrentTarget(ACharacter* Char, UBlackboardComponent* BB, FState& St)
{
	if (!Char || !BB || St.bPathLocked) return false;

	UWorld* World = Char->GetWorld(); if (!World) return false;
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return false;

	FVector Start = Char->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	FVector G;
	if (ProjectToGround(World, Start, G))      Start = G;
	if (ProjectToGround(World, TargetLoc, G))  TargetLoc = G;

	const FVector ToTarget = (TargetLoc - Start);
	const float FullDist = ToTarget.Size();
	if (FullDist < KINDA_SMALL_NUMBER) return false;

	const FVector Dir = ToTarget / FullDist;
	const float ClampedDist = FMath::Clamp(FullDist, MinChargeDistance, MaxChargeDistance);
	if (ClampedDist < KINDA_SMALL_NUMBER) return false;

	const FVector FinalDest = Start + Dir * ClampedDist;

	St.Start = Start;
	St.Dir = Dir;
	St.Distance = ClampedDist;
	St.Dest = FinalDest;

	BB->SetValueAsVector(ChargeDirectionBBKeyName, Dir);
	BB->SetValueAsVector(ChargeDestinationBBKeyName, FinalDest);
	return true;
}

void UBTT_ChargeAttack::UpdateTelegraphDecalTransform(FState& St)
{
	if (!St.DecalOwner.IsValid() || !St.Decal.IsValid()) return;

	const float Len = MaxChargeDistance;
	const FVector Mid = St.Start + St.Dir * (Len * 0.5f);
	const FRotator Rot = GroundAlignedYaw(St.Dir);

	AActor* Owner = St.DecalOwner.Get();
	Owner->SetActorLocation(Mid);
	Owner->SetActorRotation(Rot);

	St.Decal->DecalSize = FVector(DecalProjectionDepth, Len * 0.5f, DecalWidth * 0.5f);
}

void UBTT_ChargeAttack::EnterTelegraph(ACharacter* Char, FState& St, UBlackboardComponent* BB)
{
	if (UWorld* World = Char->GetWorld())
	{
		if (UDecalComponent* Decal = SpawnTelegraphDecal(World, St.Start, St.Dest))
		{
			St.Decal = Decal;
			St.DecalOwner = Decal->GetOwner();

			if (UMaterialInstanceDynamic* MID = Decal->CreateDynamicMaterialInstance())
			{
				MID->SetScalarParameterValue(TEXT("OpacityMult"), 1.f);
				St.DecalMID = MID;
			}
		}
	}

	St.PrechargeDur = 0.f;
	if (PrechargeMontage)
	{
		if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
		{
			Anim->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
			const float Played = Anim->Montage_Play(PrechargeMontage, 1.f);
			St.PrechargeDur = (Played > 0.f) ? Played : 0.6f;
		}
	}
}

void UBTT_ChargeAttack::EnterJumpUp(ACharacter* Char, FState& St)
{
	St.JumpUpDur = 0.f;
	if (JumpUpMontage)
	{
		if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
		{
			Anim->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
			const float Played = Anim->Montage_Play(JumpUpMontage, 1.f);
			St.JumpUpDur = (Played > 0.f) ? Played : 0.4f;
		}
	}
}

void UBTT_ChargeAttack::EnterDash(ACharacter* Char, FState& St)
{
	St.bPathLocked = true;

	if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		if (SprintFwdMontage) Anim->Montage_Play(SprintFwdMontage, 1.f);
	}

	if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->BrakingFrictionFactor = 0.f;
		Move->BrakingDecelerationWalking = 0.f;
		Move->GroundFriction = 0.f;
		Move->SetMovementMode(MOVE_Falling);
		Move->bUseControllerDesiredRotation = false;
	}

	if (AAIController* AICon = Cast<AAIController>(Char->GetController()))
	{
		ClearFocusForDash(AICon, Char, St);
	}

	Char->SetActorRotation(St.Dir.Rotation());
	St.DashStart = Char->GetActorLocation();
	Char->LaunchCharacter(St.Dir * ChargeSpeed, true, false);

	if (UCapsuleComponent* Cap = Char->GetCapsuleComponent())
	{
		Cap->OnComponentBeginOverlap.RemoveDynamic(this, &UBTT_ChargeAttack::OnCapsuleOverlap);
		Cap->OnComponentBeginOverlap.AddUniqueDynamic(this, &UBTT_ChargeAttack::OnCapsuleOverlap);
	}
}

void UBTT_ChargeAttack::ExitAll(ACharacter* Char, FState& St)
{
	if (UCapsuleComponent* Cap = Char->GetCapsuleComponent())
	{
		Cap->OnComponentBeginOverlap.RemoveDynamic(this, &UBTT_ChargeAttack::OnCapsuleOverlap);
	}

	if (AAIController* AICon = St.SavedAI.Get())
	{
		RestoreFocusAfterDash(AICon, nullptr, St);
	}

	if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Walking);
		Move->StopMovementImmediately();
		Move->Velocity = FVector::ZeroVector;
		Move->BrakingFrictionFactor = 2.f;
		Move->GroundFriction = 8.f;
		Move->BrakingDecelerationWalking = 2048.f;
		Move->bUseControllerDesiredRotation = true;
	}
	Char->bUseControllerRotationYaw = false;

	St.Decal = nullptr;
	St.DecalOwner = nullptr;
	Char->Tags.Remove(TEXT("ChargeHitOnce"));
}


void UBTT_ChargeAttack::ClearFocusForDash(AAIController* AICon, ACharacter* Char, FState& St)
{
	if (!AICon || !Char) return;

	St.SavedFocusActor = AICon->GetFocusActor();
	AICon->ClearFocus(EAIFocusPriority::Gameplay);
	St.bClearedFocus = true;

	AICon->SetControlRotation(St.Dir.Rotation());

	if (UPathFollowingComponent* P = AICon->GetPathFollowingComponent())
	{
		P->AbortMove(*this, FPathFollowingResultFlags::ForcedScript);
	}

	if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
	{
		Move->bUseControllerDesiredRotation = false;
	}
	Char->bUseControllerRotationYaw = true;
}

void UBTT_ChargeAttack::RestoreFocusAfterDash(AAIController* AICon, UBlackboardComponent* BB, FState& St)
{
	if (!AICon || !St.bClearedFocus) return;

	if (St.SavedFocusActor.IsValid())
	{
		AICon->SetFocus(St.SavedFocusActor.Get(), EAIFocusPriority::Gameplay);
	}
	else if (BB)
	{
		if (AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)))
		{
			AICon->SetFocus(Target, EAIFocusPriority::Gameplay);
		}
	}

	St.bClearedFocus = false;
}

bool UBTT_ChargeAttack::IsForwardPathHit(ACharacter* SelfChar, AActor* Other) const
{
	if (!SelfChar || !Other) return false;

	const UCharacterMovementComponent* Move = SelfChar->GetCharacterMovement();
	FVector Vel = Move ? Move->Velocity : FVector::ZeroVector;
	Vel.Z = 0.f;
	const FVector DirFwd = Vel.GetSafeNormal();
	if (DirFwd.IsNearlyZero()) return false;

	const FVector SelfLoc = SelfChar->GetActorLocation();
	FVector ToOther = Other->GetActorLocation() - SelfLoc;  ToOther.Z = 0.f;

	if (FVector::DotProduct(DirFwd, ToOther.GetSafeNormal()) <= 0.f) return false;

	const float Along = FVector::DotProduct(ToOther, DirFwd);
	const FVector Closest = SelfLoc + DirFwd * Along;
	const float Lateral = FVector::Dist2D(Other->GetActorLocation(), Closest);

	float HalfWidth = (HitHalfWidthOverride > 0.f) ? HitHalfWidthOverride : (DecalWidth * 0.5f);

	float OtherRadius = 0.f;
	if (const UCapsuleComponent* OtherCap = Other->FindComponentByClass<UCapsuleComponent>())
	{
		OtherRadius = OtherCap->GetScaledCapsuleRadius();
	}

	return (Lateral <= HalfWidth + FMath::Max(OtherRadius, 10.f));
}

void UBTT_ChargeAttack::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32, bool, const FHitResult&)
{
	AActor* SelfActor = OverlappedComp ? OverlappedComp->GetOwner() : nullptr;
	ACharacter* SelfChar = Cast<ACharacter>(SelfActor);
	if (!SelfChar || !Other || Other == SelfChar) return;

	if (!Other->ActorHasTag(PlayerTag)) return;

	if (!IsForwardPathHit(SelfChar, Other)) return;

	if (UBaseStatComponent* DamageComp = Other->FindComponentByClass<UBaseStatComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Charge Attack hit %s"), *Other->GetName());
		DamageComp->AddHp(-DamageAmount);
	}
	SelfChar->Tags.AddUnique(TEXT("ChargeHitOnce"));

	ForceStopCharacter(SelfChar);
}


void UBTT_ChargeAttack::ForceStopCharacter(ACharacter* Char) const
{
	if (!Char) return;

	if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		if (SprintFallMontage) Anim->Montage_Play(SprintFallMontage, 1.f);
	}

	if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->ClearAccumulatedForces();
		Move->Velocity = FVector::ZeroVector;
		Move->SetMovementMode(MOVE_None);
	}

	Char->LaunchCharacter(FVector::ZeroVector, true, true);

	if (UAnimInstance* Anim = Char->GetMesh() ? Char->GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}

	if (UCapsuleComponent* Cap = Char->GetCapsuleComponent())
	{
		Cap->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Cap->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
}