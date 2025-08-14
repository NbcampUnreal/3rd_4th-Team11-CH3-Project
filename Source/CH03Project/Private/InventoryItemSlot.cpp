#include "InventoryItemSlot.h"
#include "ItemDragDropOperation.h"
#include "DragMoveWidget.h"
#include "Components/Image.h" 
#include "Blueprint/WidgetBlueprintLibrary.h" 

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{   
    if (!bIsCanUse)
    {
        OutOperation = nullptr;
        return;
    }

	UE_LOG(LogTemp, Warning, TEXT("드래그 시작: %s"), *ItemID.ToString());
    UItemDragDropOperation* DragDropOp = NewObject<UItemDragDropOperation>(this);
    
    DragDropOp->ItemID = ItemID;
    DragDropOp->ItemType = ItemType;
    DragDropOp->ItemIcon = ItemIcon;

    if (UUserWidget* DragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualWidgetClass))
    {   
        UDragMoveWidget* DragMoveWidget = Cast<UDragMoveWidget>(DragVisual);

        if (DragMoveWidget && ItemIcon)
        {
            UImage* MyImageComponent = Cast<UImage>(DragVisual->GetWidgetFromName("DragImageMove"));

            if (MyImageComponent)
            {
                DragMoveWidget->ImageChange(ItemIcon, MyImageComponent);
            }
        }

        DragDropOp->DefaultDragVisual = DragVisual;
    }

    OutOperation = DragDropOp;
}
