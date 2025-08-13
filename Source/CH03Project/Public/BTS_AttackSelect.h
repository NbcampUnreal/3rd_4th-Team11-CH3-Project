// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_AttackSelect.generated.h"

UCLASS()
class CH03PROJECT_API UBTS_AttackSelect : public UBTService
{
    GENERATED_BODY()

public:
    UBTS_AttackSelect();

    virtual uint16 GetInstanceMemorySize() const override { return sizeof(FMem); }

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    struct FMem { bool bPrevAttacking = false; };
    FORCEINLINE FMem* M(uint8* NodeMemory) const { return reinterpret_cast<FMem*>(NodeMemory); }

    void SelectAttackByPattern(UBehaviorTreeComponent& OwnerComp, const TArray<uint8>& Pattern);
    bool IsAllowedAttack(uint8 AttackId, UBlackboardComponent* BB) const;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector PhaseKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector PhaseIndexKey;


    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector CanPrimaryKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector CanLRMKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector CanChargeKey;
    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector CanMissileKey;
    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector CanLasorKey;


    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector IsAttackingKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
    FBlackboardKeySelector AttackTypeKey;



    UPROPERTY(EditAnywhere, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
    TArray<uint8> Phase1Pattern;

    UPROPERTY(EditAnywhere, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
    TArray<uint8> Phase2Pattern;
    UPROPERTY(EditAnywhere, Category = "Pattern", meta = (AllowPrivateAccess = "true"))
    TArray<uint8> Phase3Pattern;
};