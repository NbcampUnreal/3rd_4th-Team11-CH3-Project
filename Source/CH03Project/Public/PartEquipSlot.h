#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartEquipSlot.generated.h"


UCLASS()
class CH03PROJECT_API UPartEquipSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 드래그할 아이템의 고유 ID를 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemID;

	// 아이템의 타입을 저장합니다 (예: A, B, C 파츠).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemType;

	// 아이템의 아이콘 이미지를 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class UTexture2D* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SlotImage;
	
};
