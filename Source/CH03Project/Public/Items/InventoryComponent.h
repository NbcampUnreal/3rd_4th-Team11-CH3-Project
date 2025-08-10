#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseItem.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// 아이템 추가 
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(UBaseItem* NewItem);

	// 아이템 제거 
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(UBaseItem* Item, int32 Amount = 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxInventorySize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<UBaseItem*> Items;
};