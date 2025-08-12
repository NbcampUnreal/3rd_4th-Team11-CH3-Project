#include "Items/BaseItem.h"
#include "Engine/DataTable.h"
#include "Items/ItemDataRow.h"

UBaseItem::UBaseItem()
{
    Quantity = 1;
}

FItemDataRow UBaseItem::GetItemData() const
{
    if (ItemDataHandle.DataTable)
    {
        const FItemDataRow* Row = ItemDataHandle.GetRow<FItemDataRow>(TEXT("UBaseItem::GetItemData"));
        if (Row)
        {
            return *Row;
        }
    }
    // 기본값 또는 빈 구조체 반환
    return FItemDataRow();
}

void UBaseItem::Use_Implementation(AActor* User)
{
    // 쿨타임 상태일 때
    if(bIsOnCooldown)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Item is on cooldown."), *GetName());
        return;
    }

    bIsOnCooldown = true;

    // 아이템 데이터 테이블에서 쿨타임 가져오기
    float CooldownDuration = 0.f;
    if(ItemDataHandle.DataTable && ItemDataHandle.RowName.IsValid())
    {
        FItemDataRow* ItemRow = ItemDataHandle.DataTable->FindRow<FItemDataRow>(ItemDataHandle.RowName, TEXT("Use_Implementation"));
        if(ItemRow)
        {
            CooldownDuration = ItemRow->CooldownTime;
        }
    }

    // 플레이어 존재 여부 체크
    if(User && User->GetWorld())
    {
        User->GetWorld()->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UBaseItem::ResetCooldown,
            CooldownDuration,
            false
        );
    }
}

void UBaseItem::ResetCooldown()
{
    bIsOnCooldown = false;
    UE_LOG(LogTemp, Log, TEXT("%s: Cooldown finished."), *GetName());
}