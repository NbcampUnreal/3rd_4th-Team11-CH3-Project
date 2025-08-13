// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTT_MissileAttack.generated.h"

class UAnimMontage;
class UDecalComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UBlackboardComponent;
class AAIController;
class UBaseStatComponent;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UParticleSystem;
class UBehaviorTreeComponent;

UCLASS()
class CH03PROJECT_API UBTT_MissileAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MissileAttack();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(struct FState); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	enum class EPhase : uint8 { Ready, Fire, Falling, Landing, Finish };

	struct FState
	{
		EPhase Phase = EPhase::Ready;
		float  T = 0.f;
		float MissileTravelTime = 4.0f;

		bool bDecalsPlaced = false;
		bool bExploded = false;

		int32 NumCircles = 0;

		TArray<FVector> TrackedGrounds;
		TArray<FVector> LockedImpacts;
		TArray<TWeakObjectPtr<UDecalComponent>> Decals;

		TWeakObjectPtr<ACharacter>  Boss;
		TWeakObjectPtr<AAIController> AI;
	};

	UPROPERTY(EditAnywhere, Category = "EQS", meta = (AllowPrivateAccess = "true"))
	UEnvQuery* Query = nullptr;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector NumPointsKey;
	int32 NumCircles = 0;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Ready = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Fire = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Fall = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Land = nullptr;


	UPROPERTY(EditAnywhere, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DecalMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalRadius = 300.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalThickness = 64.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalZOffset = 5.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactFX = nullptr;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float ImpactFXScaleAtRadius100 = 1.f;


	UPROPERTY(EditAnywhere, Category = "Decal", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float DelayBeforeDecals = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Decal", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float ExplosionDelay = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName PlayerTag = TEXT("Player");
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> GroundTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	bool bDestroyDecalOnFire = true;

	TWeakObjectPtr<UBehaviorTreeComponent> BTWeak;
    TWeakObjectPtr<AAIController>          AIWeak;

	FORCEINLINE FState* S(uint8* NodeMemory) const { return reinterpret_cast<FState*>(NodeMemory); }

	UPROPERTY() UEnvQueryInstanceBlueprintWrapper* ActiveQuery = nullptr;

private:
	void EnterReady(UBehaviorTreeComponent& BTC, FState& St) const;
	void EnterFire(UBehaviorTreeComponent& BTC, FState& St) const;
	void EnterFalling(UBehaviorTreeComponent& BTC, FState& St) const;
	void EnterLanding(UBehaviorTreeComponent& BTC, FState& St) const;
	void EnterFinish(UBehaviorTreeComponent& BTC, FState& St, bool bSuccess) const;

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* Wrapper, EEnvQueryStatus::Type Status);

	static bool ProjectToGround(UWorld* World, const FVector& In, FVector& Out, TEnumAsByte<ECollisionChannel> Channel, float ZOffset);
	void  SpawnDecals(FState& St, bool bLock = false) const;
	void  DestroyDecals(FState& St) const;
	void  DoImpact(FState& St) const;

	static void RandomShuffle(TArray<FVector>& Array, FRandomStream& RNG)
	{
		for (int32 i = Array.Num() - 1; i > 0; --i)
		{
			const int32 j = RNG.RandRange(0, i);
			Array.Swap(i, j);
		}
	}
};
