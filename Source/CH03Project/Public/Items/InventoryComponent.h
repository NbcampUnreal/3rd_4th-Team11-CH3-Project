#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseItem.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemChanged, FName, ItemID, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveItemChanged, FName, ItemID, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddAccessoryChanged, FName, ItemID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// 아이템 추가 (델리게이트 포함)
	FOnAddItemChanged OnAddItemChanged;
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(UBaseItem* NewItem);

	// 아이템 제거 (델리게이트 포함)
	FOnRemoveItemChanged OnRemoveItemChanged;
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(UBaseItem* Item, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool FindItem(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 CountItem(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UBaseItem* GetItem(FName ItemID);

	const TArray<UBaseItem*>& GetItems() const { return Items; }
	
	FOnAddAccessoryChanged OnAddAccessoryChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxInventorySize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<UBaseItem*> Items;
};