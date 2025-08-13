// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TravelToCenter.generated.h"

class UAnimMontage;

UCLASS()
class CH03PROJECT_API UBTT_TravelToCenter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_TravelToCenter();
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FState); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EPhase : uint8 { Ready, Jump, Travel, Fall, Land, Finish };

	struct FState
	{
		EPhase Phase = EPhase::Ready;
		float  T = 0.f;
		TWeakObjectPtr<ACharacter>  Boss;
		TWeakObjectPtr<AAIController> AI;

		FVector TargetLocation = FVector::ZeroVector;
	};

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector CenterKey;

	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Ready = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Jump = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Travel = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Fall = nullptr;
	UPROPERTY(EditAnywhere, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Land = nullptr;

	UPROPERTY(EditAnywhere, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float TravelTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float AcceptRadius = 80.f;

	FORCEINLINE FState* S(uint8* Mem) const { return reinterpret_cast<FState*>(Mem); }

private:
	void EnterReady(FState& St) const;
	void EnterJump(FState& St) const;
	void EnterTravel(FState& St) const;
	void EnterFall(FState& St) const;
	void EnterLand(FState& St) const;
	void FinishClean(UBehaviorTreeComponent& OwnerComp, FState& St, bool bSuccess);

	static bool IsPlaying(ACharacter* Char, UAnimMontage* Montage);
};
