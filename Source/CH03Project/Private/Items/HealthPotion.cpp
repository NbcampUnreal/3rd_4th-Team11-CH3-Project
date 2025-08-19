#include "Items/HealthPotion.h"
#include "GameFramework/Character.h"
#include "BaseStatComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UHealthPotion::UHealthPotion()
{
	HealAmount = 30.0f; // 초기 회복량 30
}
bool UHealthPotion::Use_Implementation(AActor* User)
{
	if(!Super::Use_Implementation(User))
	{
		return false;
	}

	if(!User)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No User!"));
		return false;
	}

	AMyCharacter* Player = Cast<AMyCharacter>(User);
	if(Player)
	{
		// 플레이어에 스탯 컴포넌트를 찾음
		UBaseStatComponent* StatComponent = Player->FindComponentByClass<UBaseStatComponent>();
		if(StatComponent)
		{
			StatComponent->HealHP(HealAmount);
			//UE_LOG(LogTemp, Warning, TEXT("HealHP!"));
		}
	}

    // 데이터테이블 핸들이 유효한지
    if(!ItemDataHandle.DataTable || !ItemDataHandle.RowName.IsValid())
    {
        return false;
    }

    // 데이터 테이블에서 Row 가져오기
    FItemDataRow* ItemRow =
        ItemDataHandle.DataTable->FindRow<FItemDataRow>(
            ItemDataHandle.RowName,
            TEXT("CatStatue Use_Implementation")
        );

    if(!ItemRow)
    {
        return false;
    }

    // 사운드 경로 설정 확인
    if(!ItemRow->UseSound.ToSoftObjectPath().IsValid())
    {
        return false;
    }

    // 메모리 상에 로드가 안 되어있으면 LoadSynchronous
    USoundBase* Sound = nullptr;
    if(ItemRow->UseSound.IsValid())
    {
        // 이미 메모리에 로드됨
        Sound = ItemRow->UseSound.Get();
    }
    else
    {
        // 로드되지 않았으면 동기 로드
        Sound = ItemRow->UseSound.LoadSynchronous();
    }

    // 재생
    if(Sound && User)
    {
        UGameplayStatics::PlaySoundAtLocation(User, Sound, User->GetActorLocation());
    }
	return true;
}

