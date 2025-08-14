// Fill out your copyright notice in the Description page of Project Settings.


#include "GunAccessory.h"
#include "InventoryItemSlot.h"

void UGunAccessory::ActivateSlot(UInventoryItemSlot* InSlot, FName InItemID)
{
    if (!InSlot) return;

    InSlot->ItemID = InItemID;
    InSlot->bIsCanUse = true;
}

void UGunAccessory::UpdateAccessory(FName ItemID)
{
    if (ItemID == "EMUp")
    {
        ActivateSlot(EM0, ItemID);
    }
    else if (ItemID == "EMDown")
    {
        ActivateSlot(EM1, ItemID);
    }
    if (ItemID == "AHUp")
    {
        ActivateSlot(EM0, ItemID);
    }
    else if (ItemID == "AHDown")
    {
        ActivateSlot(EM1, ItemID);
    }
    if (ItemID == "SLUp")
    {
        ActivateSlot(EM0, ItemID);
    }
    else if (ItemID == "SLDown")
    {
        ActivateSlot(EM1, ItemID);
    }
}