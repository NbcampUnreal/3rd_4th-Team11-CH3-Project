#include "Items/CatStatue.h"
#include "Items/ItemDataRow.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UCatStatue::Use_Implementation(AActor* User)
{
    Super::Use_Implementation(User);

    // 데이터테이블 핸들이 유효한지
    if(!ItemDataHandle.DataTable || !ItemDataHandle.RowName.IsValid())
    {
        //UE_LOG(LogTemp, Warning, TEXT("CatStatue: No valid ItemDataHandle"));
        return;
    }

    // 데이터 테이블에서 Row 가져오기
    FItemDataRow* ItemRow =
        ItemDataHandle.DataTable->FindRow<FItemDataRow>(
            ItemDataHandle.RowName,
            TEXT("CatStatue Use_Implementation")
        );

    if(!ItemRow)
    {
        //UE_LOG(LogTemp, Warning, TEXT("CatStatue: Row not found for %s"), *ItemDataHandle.RowName.ToString());
        return;
    }

    // 사운드 경로 설정 확인
    if(!ItemRow->PickupSound.ToSoftObjectPath().IsValid())
    {
        //UE_LOG(LogTemp, Warning, TEXT("CatStatue: PickupSound path is not set for %s"), *ItemDataHandle.RowName.ToString());
        return;
    }

    // 메모리 상에 로드가 안 되어있으면 LoadSynchronous
    USoundBase* Sound = nullptr;
    if(ItemRow->PickupSound.IsValid())
    {
        // 이미 메모리에 로드됨
        Sound = ItemRow->PickupSound.Get();
    }
    else
    {
        // 로드되지 않았으면 동기 로드
        Sound = ItemRow->PickupSound.LoadSynchronous();
    }

    // 재생
    if(Sound && User)
    {
        UGameplayStatics::PlaySoundAtLocation(User, Sound, User->GetActorLocation());
        UE_LOG(LogTemp, Log, TEXT("CatStatue: Played sound %s"), *Sound->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CatStatue: Failed to load or play sound for %s"), *ItemDataHandle.RowName.ToString());
    }
}