#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"


UCLASS()
class CH03PROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	


public:
    // 드래그할 아이템의 고유 ID를 저장합니다.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    FName ItemID;

    // 아이템의 타입을 저장합니다 (예: A, B, C 파츠).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    FName ItemType;

    // 아이템의 아이콘 이미지를 저장합니다.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
    class UTexture2D* ItemIcon;

};
