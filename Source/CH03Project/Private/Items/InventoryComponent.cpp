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

	// 같은 아이템이 이미 있는지 확인
	for(UBaseItem* Item : Items)
	{
		if(Item && Item->ItemDataHandle.RowName == NewItem->ItemDataHandle.RowName)
		{
			const FItemDataRow Data = Item->GetItemData();
			if(Item->Quantity < Data.MaxStackSize)
			{
				Item->Quantity = FMath::Min(Item->Quantity + NewItem->Quantity, Data.MaxStackSize);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Max stack reached for %s"), *Data.ItemName.ToString());
				return false;
			}
		}
	}

	// 새 아이템 추가
	if(Items.Num() < MaxInventorySize)
	{
		Items.Add(NewItem);
		return true;
	}

	// 인벤토리 가득 참
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