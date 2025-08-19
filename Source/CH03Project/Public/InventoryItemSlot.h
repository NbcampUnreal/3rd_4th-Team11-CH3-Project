#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UImage;
UCLASS()
class CH03PROJECT_API UInventoryItemSlot : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FName ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    class UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
    TSubclassOf<UUserWidget> DragVisualWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    bool bIsCanUse = false;

    UPROPERTY(meta = (BindWidgetOptional))
    UImage* SlotImage;

    void ShowSlotImage();
};
