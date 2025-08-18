// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"


class UAnimMontage;
class UDecalComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UBlackboardComponent;
class AAIController;	

UCLASS()
class CH03PROJECT_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ChargeAttack();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(struct FState); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EPhase : uint8 { Ready, JumpUp, Dash, Finish };

	struct FState
	{
		EPhase  Phase = EPhase::Ready;
		float   T = 0.f;
		FVector Start;
		FVector Dest;
		FVector Dir;
		float   Distance = 0.f;
		float   PrechargeDur = 0.f;
		float   JumpUpDur = 0.f;
		FVector DashStart;
		bool    bHitOnce = false;

		TWeakObjectPtr<UDecalComponent>          Decal;
		TWeakObjectPtr<AActor>                   DecalOwner;
		TWeakObjectPtr<UMaterialInstanceDynamic> DecalMID;

		TWeakObjectPtr<AAIController> SavedAI;
		TWeakObjectPtr<AActor>        SavedFocusActor;
		bool                          bClearedFocus = false;

		bool                          bPathLocked = false;
		bool						  bEnded = false;	
	};

	FORCEINLINE FState* S(uint8* NodeMemory) const { return reinterpret_cast<FState*>(NodeMemory); }

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FName ChargeDirectionBBKeyName = TEXT("ChargeDirection");
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FName ChargeDestinationBBKeyName = TEXT("ChargeDestination");


	UPROPERTY(EditAnywhere, Category = "Charge", meta = (ClampMin = "0.0", AllowPrivateAccess = "true"))
	float MinChargeDistance = 300.f;
	UPROPERTY(EditAnywhere, Category = "Charge", meta = (ClampMin = "100.0", AllowPrivateAccess = "true"))
	float MaxChargeDistance = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Charge", meta = (ClampMin = "100.0", AllowPrivateAccess = "true"))
	float ChargeSpeed = 3000.f;
	UPROPERTY(EditAnywhere, Category = "Charge", meta = (AllowPrivateAccess = "true"))
	float GraceTimeAfterHit = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount = 30.f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName PlayerTag = TEXT("Player");


	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PrechargeMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpUpMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SprintFwdMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SprintFallMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "FX", meta = (ClampMin = "10.0", AllowPrivateAccess = "true"))
	float DecalWidth = 100.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (ClampMin = "1.0", AllowPrivateAccess = "true"))
	float DecalProjectionDepth = 64.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float DecalLifeTime = 4.0f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float DecalBlinkHz = 6.0f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	float DecalYawOffset = 90.f;
	UPROPERTY(EditAnywhere, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* TelegraphDecalMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Charge", meta = (AllowPrivateAccess = "true"))
	float HitHalfWidthOverride = -1.f;
	UPROPERTY(EditAnywhere, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	float GroundProbeUp = 50.f;
	UPROPERTY(EditAnywhere, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	float GroundProbeDown = 600.f;

private:
	bool     ProjectToGround(UWorld* World, const FVector& InLoc, FVector& OutLoc) const;
	FRotator GroundAlignedYaw(const FVector& Dir) const;
	UDecalComponent* SpawnTelegraphDecal(UWorld* World, const FVector& Start, const FVector& End) const;

	bool RecomputePathFromCurrentTarget(class ACharacter* Char, class UBlackboardComponent* BB, struct FState& St);
	void UpdateTelegraphDecalTransform(struct FState& St);

	void EnterTelegraph(class ACharacter* Char, FState& St, UBlackboardComponent* BB);
	void EnterJumpUp(class ACharacter* Char, FState& St);
	void EnterDash(class ACharacter* Char, FState& St);
	void EndCharge(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result);
	void ExitAll(class ACharacter* Char, FState& St);

	void ClearFocusForDash(class AAIController* AICon, class ACharacter* Char, FState& St);
	void RestoreFocusAfterDash(class AAIController* AICon, class UBlackboardComponent* BB, FState& St);

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsForwardPathHit(class ACharacter* SelfChar, class AActor* Other) const;
	void ForceStopCharacter(class ACharacter* Char) const;
};
