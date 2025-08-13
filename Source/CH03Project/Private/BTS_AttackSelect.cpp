// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_AttackSelect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTS_AttackSelect::UBTS_AttackSelect()
{
    NodeName = "Attack Selector";
    bCreateNodeInstance = true;

    Phase1Pattern = { 1, 2, 1, 3, 3, 2, 1, 1, 2 }; // 1=Primary, 2=LRM, 3=Charge
    Phase2Pattern = { 4, 1, 3, 3, 1, 4, 1, 3, 4 }; // 4=Missile
    Phase3Pattern = { 3, 2, 1, 4, 3, 3, 1, 2, 5 }; // 5=Lasor
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
        const int32 N = Phase1Pattern.Num();
        Index = (N > 0) ? (Index + 1) % N : 0;
        BB->SetValueAsInt(PhaseIndexKey.SelectedKeyName, Index);
    }
    Mem->bPrevAttacking = bAttacking;

    if (bAttacking) return;

    const int32 Phase = BB->GetValueAsInt(PhaseKey.SelectedKeyName);
    switch (Phase)
    {
    case 1:
    {
        SelectAttackByPattern(OwnerComp, Phase1Pattern); 
        break;
    }
    case 2:
    {
        SelectAttackByPattern(OwnerComp, Phase2Pattern);
        break;
    }
    case 3:
    {
		SelectAttackByPattern(OwnerComp, Phase3Pattern);
		break;
    }
    default:
    {
        SelectAttackByPattern(OwnerComp, Phase1Pattern);
        break;
    }
    }
}

void UBTS_AttackSelect::SelectAttackByPattern(UBehaviorTreeComponent& OwnerComp, const TArray<uint8>& Pattern)
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;
    if (BB->GetValueAsBool(IsAttackingKey.SelectedKeyName)) return;

    const int32 N = Pattern.Num();
    if (N == 0)
    {
        BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, 0);
        return;
    }

    const int32 BaseIndex = BB->GetValueAsInt(PhaseIndexKey.SelectedKeyName);

    for (int32 i = 0; i < N; ++i)
    {
        const int32 TryIndex = (BaseIndex + i) % N;
        const uint8 Attack = Pattern[TryIndex];

        if (IsAllowedAttack(Attack, BB))
        {
            BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, Attack);
            return;
        }
    }

    BB->SetValueAsEnum(AttackTypeKey.SelectedKeyName, 0);
}

bool UBTS_AttackSelect::IsAllowedAttack(uint8 AttackId, UBlackboardComponent* BB) const
{
    switch (AttackId)
    {
    case 1: return BB->GetValueAsBool(CanPrimaryKey.SelectedKeyName);
    case 2: return BB->GetValueAsBool(CanLRMKey.SelectedKeyName);
    case 3: return BB->GetValueAsBool(CanChargeKey.SelectedKeyName);
    case 4: return BB->GetValueAsBool(CanMissileKey.SelectedKeyName);
    case 5: return BB->GetValueAsBool(CanLasorKey.SelectedKeyName); 
    default: return false;
    }
}