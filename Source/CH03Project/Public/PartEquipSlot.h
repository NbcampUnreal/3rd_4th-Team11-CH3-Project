#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragDropOperation.h"
#include "PartEquipSlot.generated.h"

class UWeaponAccCompoenent;
UCLASS()
class CH03PROJECT_API UPartEquipSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	TObjectPtr<UTexture2D> DefaultEmptyImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemType;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class UTexture2D* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SlotImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> DragVisualWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "PartEquipSlot")
	void SetSlotItem(const FName NewItemID, UTexture2D* NewItemIcon);

	UFUNCTION(BlueprintCallable, Category = "PartEquipSlot")
	void ClearSlot();


	UWeaponAccCompoenent* WeaponAccCompoenent;
	void SearchWAC();

	//착용효과들
	void ResetEffectAH();
	void ResetEffectEM();
	void ResetEffectSL();

	void SetEffectAH01(bool bIsPlus);
	void SetEffectAH02(bool bIsPlus);
	void SetEffectEM01(bool bIsPlus);
	void SetEffectEM02(bool bIsPlus);
	void SetEffectSL01(bool bIsPlus);
	void SetEffectSL02(bool bIsPlus);


	
};
