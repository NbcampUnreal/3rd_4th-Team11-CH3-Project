#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryItemSlot.generated.h"


UCLASS()
class CH03PROJECT_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    // 드래그할 아이템의 고유 ID를 저장합니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FName ItemID;

    // 아이템의 타입을 저장합니다 (예: A, B, C 파츠).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FName ItemType;

    // 아이템의 아이콘 이미지를 저장합니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    class UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
    TSubclassOf<UUserWidget> DragVisualWidgetClass;
};
