#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UPartEquipSlot;


UCLASS()
class CH03PROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	


public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    FName ItemID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    FName ItemType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    class UTexture2D* ItemIcon;

    UPROPERTY()
    UPartEquipSlot* SourceSlot;

};
