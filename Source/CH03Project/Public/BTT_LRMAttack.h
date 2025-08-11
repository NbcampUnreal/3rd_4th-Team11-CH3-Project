// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LRMAttack.generated.h"

class UAnimMontage;
class UDecalComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UBlackboardComponent;
class AAIController;
class UParticleSystem;

UCLASS()
class CH03PROJECT_API UBTT_LRMAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_LRMAttack();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(struct FState); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	enum class EPhase : uint8 { Ready, Tracking, Fire, Falling, Landing, Finish };
	
	struct FState
	{
		EPhase Phase = EPhase::Ready;
		float  T = 0.f;
		bool bImpactLocked = false;
		float MissileTravelTime = 5.0f;

		FVector TrackedGround = FVector::ZeroVector;
		FVector LockedImpact = FVector::ZeroVector;

		TWeakObjectPtr<UDecalComponent>          Decal;
		TWeakObjectPtr<UMaterialInstanceDynamic> DecalMID;

		TWeakObjectPtr<ACharacter>  Boss;
		TWeakObjectPtr<AAIController> AI;
	};

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Ready = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_JumpFire = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_JumpFall = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_JumpLand = nullptr;

	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DecalMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalRadius = 100.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalThickness = 64.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalZOffset = 5.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactFX = nullptr;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float ImpactFXScaleAtRadius100 = 1.f;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 40.f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName PlayerTag = TEXT("Player");

	UPROPERTY(EditAnywhere, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> GroundTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	bool bFollowTargetWhileTracking = true;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	bool bDestroyDecalOnFire = true;

	FORCEINLINE FState* S(uint8* NodeMemory) const { return reinterpret_cast<FState*>(NodeMemory); }

private:
	void EnterReady(FState& St) const;
	void EnterTracking(FState& St) const;
	void EnterFalling(FState& St) const;
	void EnterLanding(FState& St) const;
	void FinishClean(UBehaviorTreeComponent& OwnerComp, FState& St, bool bSuccess);

	bool  UpdateTrackedGround(FState& St) const;
	void  SpawnOrUpdateDecal(FState& St, bool bLock = false) const;
	void  DestroyDecal(FState& St) const;
	void  DoImpact(FState& St) const;

	mutable FDelegateHandle MontageEndedHandle;
	bool bBoundNotify = false;
};
