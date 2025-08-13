#include "PartEquipSlot.h"
#include "ItemDragDropOperation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

bool UPartEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("드롭 시도: %s"), *InOperation->GetName());
   Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UItemDragDropOperation* DraggedItemOp = Cast<UItemDragDropOperation>(InOperation);
    if (!DraggedItemOp)
    {
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("드롭 시도2"));
    if (DraggedItemOp->ItemType == ItemType)
    {
        UE_LOG(LogTemp, Warning, TEXT("드롭 시도3"));
        // 드래그 오퍼레이션에 들어있는 아이템 아이콘으로 슬롯 이미지 변경
            if (this->SlotImage)
            {
                this->SlotImage->SetBrushFromTexture(DraggedItemOp->ItemIcon);
                UE_LOG(LogTemp, Warning, TEXT("아이템 장착 성공: 타입 일치"));
                return true; // 드롭 성공
            }
        //이 슬롯의 아이콘을 드롭된 아이템의 아이콘으로 변경
        //UpdateSlotImage(DraggedItemOp->ItemIcon);

        // 캐릭터에 장착 효과 적용
        // AMyCharacter* MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        // if (MyCharacter)
        // {
        //     MyCharacter->EquipPart(DraggedItemOp->ItemID, ItemType);
        // }

        return true; // 드롭 성공
    }

    return false; // 검증 실패
}

bool UPartEquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return true; // 여기서 true를 반환해야 드롭 가능
}