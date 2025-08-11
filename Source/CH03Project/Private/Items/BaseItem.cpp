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
    
}