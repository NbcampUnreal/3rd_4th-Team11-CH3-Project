// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_AttackSelect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTS_AttackSelect::UBTS_AttackSelect()
{
    NodeName = "Attack Selector";
    bCreateNodeInstance = true;

    Phase1Pattern = { 1, 2, 1, 3, 3, 2, 1, 1, 2 }; // 1=Primary, 2=LRM, 3=Charge
}

void UBTS_AttackSelect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    FMem* Mem = M(NodeMemory);
    const bool bAttacking = BB->GetValueAsBool(IsAttackingKey.SelectedKeyName);

    if (Mem->bPrevAttacking && !bAttacking)
    {
        int32 Index = BB->GetValueAsInt(PhaseIndexKey.SelectedKeyName);
        if (Phase1Pattern.Num() > 0)
        {
            Index = (Index + 1) % Phase1Pattern.Num();
        }
        else
        {
            Index = 0;
        }
        BB->SetValueAsInt(PhaseIndexKey.SelectedKeyName, Index);
    }
    Mem->bPrevAttacking = bAttacking;

    if (bAttacking)
        return;

    const int32 Phase = BB->GetValueAsInt(PhaseKey.SelectedKeyName);
    if (Phase == 1)
    {
        SelectAttackPhase1(OwnerComp);
    }
}


void UBTS_AttackSelect::SelectAttackPhase1(UBehaviorTreeComponent& OwnerComp)
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    if (BB->GetValueAsBool(IsAttackingKey.SelectedKeyName))
        return;

    const int32 N = Phase1Pattern.Num();
    if (N == 0)
    {
        BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, 0);
        return;
    }

    const bool bCanPrimary = BB->GetValueAsBool(CanPrimaryKey.SelectedKeyName);
    const bool bCanLRM = BB->GetValueAsBool(CanLRMKey.SelectedKeyName);
    const bool bCanCharge = BB->GetValueAsBool(CanChargeKey.SelectedKeyName);

    const int32 BaseIndex = BB->GetValueAsInt(PhaseIndexKey.SelectedKeyName);

    for (int32 i = 0; i < N; ++i)
    {
        const int32 TryIndex = (BaseIndex + i) % N;
        const uint8 Attack = Phase1Pattern[TryIndex];

        bool bAllowed = false;
        if (Attack == 1 && bCanPrimary) bAllowed = true;
        if (Attack == 2 && bCanLRM)     bAllowed = true;
        if (Attack == 3 && bCanCharge)  bAllowed = true;

        if (bAllowed)
        {
            BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, Attack);
            return;
        }
    }
    BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, 0);
}
