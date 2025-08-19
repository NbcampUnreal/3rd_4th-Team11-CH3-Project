#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunAccessory.generated.h"

class UInventoryItemSlot;
class UPartEquipSlot;

UCLASS()
class CH03PROJECT_API UGunAccessory : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	//-----------------inv---------------
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* SL0;
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* SL1;
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* AH0;
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* AH1;
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* EM0;
	UPROPERTY(meta = (BindWidget))
	UInventoryItemSlot* EM1;
	//----------equickslot------------
	UPROPERTY(meta = (BindWidget))
	UPartEquipSlot* SLSlot;
	UPROPERTY(meta = (BindWidget))
	UPartEquipSlot* AHSlot;
	UPROPERTY(meta = (BindWidget))
	UPartEquipSlot* EMSlot;

	bool bIsCanUse;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateAccessory(FName ItemID);

protected:
	void ActivateSlot(UInventoryItemSlot* InSlot);
};
