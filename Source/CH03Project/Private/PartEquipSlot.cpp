#include "PartEquipSlot.h"
#include "ItemDragDropOperation.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Image.h"


void UPartEquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (ItemID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("슬롯에 아이템이 없어 드래그를 시작할 수 없습니다."));
        OutOperation = nullptr;
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("드래그 시작: %s"), *ItemID.ToString());

    
    UItemDragDropOperation* DragDropOp = NewObject<UItemDragDropOperation>(this);
    if (!DragDropOp)
    {
        OutOperation = nullptr;
        return;
    }
        
    DragDropOp->ItemID = ItemID;
    DragDropOp->ItemType = ItemType;
    DragDropOp->ItemIcon = ItemIcon;
    DragDropOp->SourceSlot = this;


    if (DragVisualWidgetClass)
    {
        if (UUserWidget* DragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualWidgetClass))
        {
            if (UImage* VisualImage = Cast<UImage>(DragVisual->GetWidgetFromName(TEXT("DragImageMove"))))
            {
                VisualImage->SetBrushFromTexture(ItemIcon);
            }
            DragDropOp->DefaultDragVisual = DragVisual;
        }
    }

    OutOperation = DragDropOp;
}




bool UPartEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UItemDragDropOperation* DraggedItemOp = Cast<UItemDragDropOperation>(InOperation);
    if (!DraggedItemOp)
    {
        return false;
    }

    if (DraggedItemOp->ItemType == ItemType)
    {
        if (DraggedItemOp->SourceSlot && DraggedItemOp->SourceSlot != this)
        {
            DraggedItemOp->SourceSlot->ClearSlot();
        }
        
        SetSlotItem(DraggedItemOp->ItemID, DraggedItemOp->ItemIcon);
        return true;
    }

    return false;
}

bool UPartEquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return true;
}


void UPartEquipSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

    
    if (InOperation && Cast<UItemDragDropOperation>(InOperation))
    {
        this->ClearSlot();
    }
}


void UPartEquipSlot::SetSlotItem(const FName NewItemID, UTexture2D* NewItemIcon)
{
	UE_LOG(LogTemp, Warning, TEXT("슬롯에 아이템 설정: %s"), *NewItemID.ToString());
    ItemID = NewItemID;
    ItemIcon = NewItemIcon;
    if (SlotImage)
    {
        SlotImage->SetBrushFromTexture(ItemIcon);
    }

    if (ItemID == "EM01")
    {
        SetEffectEM01();
    }
    else if (ItemID == "EM02")
    {
        SetEffectEM02();
    }
    else if (ItemID == "AH01")
    {
        SetEffectAH01();
    }
    else if (ItemID == "AH02")
    {
        SetEffectAH02();
    }
    else if (ItemID == "SL01")
    {
        SetEffectSL01();
    }
    else if (ItemID == "SL02")
    {
		SetEffectSL02();
    }
}


void UPartEquipSlot::ClearSlot()
{
	UE_LOG(LogTemp, Warning, TEXT("슬롯 아이템 초기화"));
    ItemID = NAME_None; // 아이템 ID를 초기화
    ItemIcon = nullptr; // 아이콘 레퍼런스를 해제
    if (SlotImage)
    {
        SlotImage->SetBrushFromTexture(DefaultEmptyImage);
    }

    if (ItemType=="EM")
    {
        ResetEffectEM();
    }
    else if (ItemType == "AH")
    {
        ResetEffectAH();
    }
    else if (ItemType == "SL")
    {
        ResetEffectSL();
	}
}


//효과구현
void UPartEquipSlot::ResetEffectAH()
{
    // AH 효과 초기화 로직
    UE_LOG(LogTemp, Warning, TEXT("AH 효과 초기화"));
}

void UPartEquipSlot::ResetEffectEM()
{
    // EM 효과 초기화 로직
    UE_LOG(LogTemp, Warning, TEXT("EM 효과 초기화"));
}
void UPartEquipSlot::ResetEffectSL()
{
    // SL 효과 초기화 로직
    UE_LOG(LogTemp, Warning, TEXT("SL 효과 초기화"));
}

void UPartEquipSlot::SetEffectAH01()
{
    // AH01 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("AH01 효과 적용"));
}

void UPartEquipSlot::SetEffectAH02()
{
    // AH02 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("AH02 효과 적용"));
}

void UPartEquipSlot::SetEffectEM01()
{
    // EM01 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("EM01 효과 적용"));
}

void UPartEquipSlot::SetEffectEM02()
{
    // EM02 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("EM02 효과 적용"));
}

void UPartEquipSlot::SetEffectSL01()
{
    // SL01 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("SL01 효과 적용"));
}

void UPartEquipSlot::SetEffectSL02()
{
    // SL02 효과 적용 로직
    UE_LOG(LogTemp, Warning, TEXT("SL02 효과 적용"));
}

