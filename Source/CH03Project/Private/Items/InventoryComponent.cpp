#include "Items/InventoryComponent.h"
#include "Engine/DataTable.h"

UInventoryComponent::UInventoryComponent()
{
	MaxInventorySize = 20; // 기본 인벤토리 칸
}

bool UInventoryComponent::AddItem(UBaseItem* NewItem)
{
    if(!NewItem)
        return false;

    // 같은 아이템 여부 확인
    for(UBaseItem* Item : Items)
    {
        if(Item && Item->ItemDataHandle.RowName == NewItem->ItemDataHandle.RowName)
        {
            // 수량 증가
            Item->Quantity += NewItem->Quantity;

            UE_LOG(LogTemp, Log, TEXT("[%s] 수량 증가: %d"),
                   *Item->GetItemData().ItemName.ToString(),
                   Item->Quantity
            );
            return true;
        }
    }

    // 새 아이템 추가
    if(Items.Num() < MaxInventorySize)
    {
        Items.Add(NewItem);
        UE_LOG(LogTemp, Log, TEXT("[%s] 수량 증가: %d"),
               *NewItem->GetItemData().ItemName.ToString(),
               NewItem->Quantity
        );
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
    return false;
}

bool UInventoryComponent::RemoveItem(UBaseItem* Item, int32 Amount)
{
	if(!Item)
		return false;

	if(Item->Quantity > Amount)
	{
		Item->Quantity -= Amount;
		return true;
	}
	else
	{
		Items.Remove(Item);
		return true;
	}
}