// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LasorAttack.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class UNiagaraComponent;
class UBaseStatComponent;

UCLASS()
class CH03PROJECT_API UBTT_LasorAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_LasorAttack();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(struct FState); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	enum class EPhase : uint8 {Ready, Turn, Finish};

	struct FState
	{
		EPhase Phase = EPhase::Ready;
		float T = 0.f;
		bool   bTurnRight = true;

		float  StartYaw = 0.f;
		float  TargetYawOffset = 0.f;
		float  BeamLen = 0.f;

		TWeakObjectPtr<ACharacter>   Boss;
		TWeakObjectPtr<AAIController> AI;
		TWeakObjectPtr<UNiagaraComponent> LaserNS;
	};

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetActoryKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector CenterKey;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_lft = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_rt = nullptr;

	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* LaserSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocket = TEXT("Muzzle_01");

	UPROPERTY(EditAnywhere, Category = "Laser") 
	FName BeamStartParam = TEXT("Beam Start001");
	UPROPERTY(EditAnywhere, Category = "Laser") 
	FName BeamEndParam = TEXT("Beam End");


	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float Duration = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float AngularSpeedDeg = 90.f;
	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float LaserRange = 4000.f;
	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float BeamDamageRadius = 25.f;
	UPROPERTY(EditAnywhere, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float BeamExtendSpeed = 6000.f;


	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmountPS = 30.f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName PlayerTag = TEXT("Player");

	UPROPERTY(EditAnywhere, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	TWeakObjectPtr<UBehaviorTreeComponent> BTWeak;

	FORCEINLINE FState* S(uint8* NodeMemory) const { return reinterpret_cast<FState*>(NodeMemory); }

private:
	void EnterReady(FState& St) const;
	void EnterTurn(FState& St) const;
	void EnterFinish(UBehaviorTreeComponent& BTC, FState& St, bool bSuccess) const;

	void StartLaser(FState& St) const;
	void StopLaser(FState& St) const;
	void UpdateLaserAndDamage(FState& St, float DeltaSeconds) const;

	static void SetNiagaraVec3(UNiagaraComponent* NC, const FName& Param, const FVector& V);

	static void ApplyTransDamageOrFallback(AActor* Target, float Amount);
};
