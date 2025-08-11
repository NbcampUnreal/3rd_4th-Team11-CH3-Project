#include "Items/CatStatue.h"
#include "Items/ItemDataRow.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UCatStatue::Use_Implementation(AActor* User)
{
    if(!ItemDataHandle.DataTable || !ItemDataHandle.RowName.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("CatStatue: No valid ItemDataHandle"));
        return;
    }

    // 데이터 테이블에서 Row 가져오기
    FItemDataRow* ItemRow = 
        ItemDataHandle.DataTable->FindRow<FItemDataRow>(
            ItemDataHandle.RowName,
            TEXT("CatStatue Use_Implementation")
    );

    if(ItemRow && ItemRow->PickupSound.IsValid())
    {
        // 소프트 레퍼런스 로드방법
        USoundBase* Sound = ItemRow->PickupSound.LoadSynchronous();
        if(Sound && User)
        {
            UGameplayStatics::PlaySoundAtLocation(User, Sound, User->GetActorLocation());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CatStatue: PickupSound not found in ItemData"));
    }
}