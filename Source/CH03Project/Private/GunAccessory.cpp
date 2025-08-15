// Fill out your copyright notice in the Description page of Project Settings.


#include "GunAccessory.h"
#include "InventoryItemSlot.h"

void UGunAccessory::ActivateSlot(UInventoryItemSlot* InSlot)
{
    if (!InSlot) return;

    //InSlot->ItemID = InItemID;
    InSlot->bIsCanUse = true;
}

void UGunAccessory::UpdateAccessory(FName ItemID)
{
    if (ItemID == "EM01")
    {
        ActivateSlot(EM0);
    }
    else if (ItemID == "EM02")
    {
        ActivateSlot(EM1);
    }
    if (ItemID == "AH01")
    {
        ActivateSlot(EM0);
    }
    else if (ItemID == "AH02")
    {
        ActivateSlot(EM1);
    }
    if (ItemID == "SL01")
    {
        ActivateSlot(EM0);
    }
    else if (ItemID == "SL02")
    {
        ActivateSlot(EM1);
    }
}