#include "PartEquipSlot.h"
#include "ItemDragDropOperation.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Components/Image.h"
#include "WeaponAccCompoenent.h"
#include "BaseWeapon.h"



void UPartEquipSlot::NativeConstruct()
{   
	Super::NativeConstruct();
    if (ItemIcon == nullptr)
    {
        SlotImage->SetBrushFromTexture(DefaultEmptyImage);
    }
    
}


void UPartEquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (ItemID.IsNone())
    {
        OutOperation = nullptr;
        return;
    }

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
    ClearSlot();

    ItemID = NewItemID;
    ItemIcon = NewItemIcon;
    if (SlotImage)
    {
        SlotImage->SetBrushFromTexture(ItemIcon);
    }

    if (!WeaponAccCompoenent)
    {
        SearchWAC();
    }

	SetEffects(ItemID, true);
}


void UPartEquipSlot::ClearSlot()
{
    if (!WeaponAccCompoenent)
    {
        SearchWAC();
    }

    if (SlotImage)
    {
        SlotImage->SetBrushFromTexture(DefaultEmptyImage);
    }

    SetEffects(ItemID, false);
    

    ItemID = NAME_None; // 아이템 ID를 초기화
    ItemIcon = nullptr; // 아이콘 레퍼런스를 해제
}

void UPartEquipSlot::SearchWAC()
{   
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (PlayerCharacter)
    {
        AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerCharacter);

        if (MyCharacter && MyCharacter->GetEquippedWeapon())
        {   
            UWeaponAccCompoenent* FoundComponent = MyCharacter->GetEquippedWeapon()->FindComponentByClass<UWeaponAccCompoenent>();

            if (FoundComponent)
            {
                WeaponAccCompoenent = FoundComponent;
            }
        }
    }
}

//효과구현
//void UPartEquipSlot::ResetEffectAH()
//{   
//    UE_LOG(LogTemp, Warning, TEXT("AH 효과 초기화"));
//}
//
//void UPartEquipSlot::ResetEffectEM()
//{
//    UE_LOG(LogTemp, Warning, TEXT("EM 효과 초기화"));
//}
//void UPartEquipSlot::ResetEffectSL()
//{
//    UE_LOG(LogTemp, Warning, TEXT("SL 효과 초기화"));
//}

void UPartEquipSlot::SetEffectAH01(bool bIsPlus)
{
    //불값 표시
    UE_LOG(LogTemp, Warning, TEXT("AH01 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
		WeaponAccCompoenent->PlusHandle -= 20;
    }
    else
    {
        WeaponAccCompoenent->PlusHandle += 20;
    }
}

void UPartEquipSlot::SetEffectAH02(bool bIsPlus)
{
    UE_LOG(LogTemp, Warning, TEXT("AH02 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
        WeaponAccCompoenent->PlusHandle -= 50;
    }
    else
    {
        WeaponAccCompoenent->PlusHandle += 50;
    }
}

void UPartEquipSlot::SetEffectEM01(bool bIsPlus)
{
    UE_LOG(LogTemp, Warning, TEXT("EM01 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
        WeaponAccCompoenent->PlusAmmo += 10;
    }
    else
    {
        WeaponAccCompoenent->PlusAmmo -= 10;
    }
}

void UPartEquipSlot::SetEffectEM02(bool bIsPlus)
{
    UE_LOG(LogTemp, Warning, TEXT("EM02 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
        WeaponAccCompoenent->PlusAmmo += 25;
    }
    else
    {
        WeaponAccCompoenent->PlusAmmo -= 25;
    }
}

void UPartEquipSlot::SetEffectSL01(bool bIsPlus)
{
    UE_LOG(LogTemp, Warning, TEXT("SL01 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
        WeaponAccCompoenent->PlusAttack += 3;
    }
    else
    {
        WeaponAccCompoenent->PlusAttack -= 3;
    }
}

void UPartEquipSlot::SetEffectSL02(bool bIsPlus)
{
    UE_LOG(LogTemp, Warning, TEXT("SL02 효과 적용: %s"), bIsPlus ? TEXT("True") : TEXT("False"));
    if (bIsPlus)
    {
        WeaponAccCompoenent->PlusAttack += 8;
    }
    else
    {
        WeaponAccCompoenent->PlusAttack -= 8;
    }
}


void UPartEquipSlot::SetEffects(FName ItemIDs, bool bIsPlus)
{
    if (ItemIDs == "EM01")
    {
        SetEffectEM01(bIsPlus);
    }
    else if (ItemIDs == "EM02")
    {
        SetEffectEM02(bIsPlus);
    }
    else if (ItemIDs == "AH01")
    {
        SetEffectAH01(bIsPlus);
    }
    else if (ItemIDs == "AH02")
    {
        SetEffectAH02(bIsPlus);
    }
    else if (ItemIDs == "SL01")
    {
        SetEffectSL01(bIsPlus);
    }
    else if (ItemIDs == "SL02")
    {
        SetEffectSL02(bIsPlus);
	}
    WeaponAccCompoenent->UpdateWeaponAcc();
}
