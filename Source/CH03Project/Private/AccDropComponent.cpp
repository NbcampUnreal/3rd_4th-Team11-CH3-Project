#include "AccDropComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameStatePlay.h"
#include "Items/PickupItem.h"

UAccDropComponent::UAccDropComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UAccDropComponent::DeathEnemy()
{
	int32 RandomValue = FMath::RandRange(0, 100);
	if (RandomValue <= ChanceToDrop)
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 드랍되었습니다. 확률: %d%%, 랜덤값: %d"), ChanceToDrop, RandomValue);
		DropItem();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템이 드랍되지 않았습니다. 확률: %d%%, 랜덤값: %d"), ChanceToDrop, RandomValue);
	}
}


void UAccDropComponent::DropItem()
{
	//게임스테이트를 참조해서 드랍되지 않은 아이템 중 하나 드랍함ㅇㅋ?
	//일단 게임스테이트 참조
	AGameStatePlay* GameState = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState)
	{	
		UE_LOG(LogTemp, Warning, TEXT("게임스테이트를 찾을 수 없습니다."));
		return;
	}
	if (GameState->AccItemCanGetList.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("드랍 가능한 아이템이 없습니다."));
		return;
	}

	//AccItemCanGetList카운트 중 랜덤으로 1개 뽑음
	int32 RandomIndex = FMath::RandRange(0, GameState->AccItemCanGetList.Num() - 1);
	int32 DroppedItem = GameState->AccItemCanGetList[RandomIndex];

	if (GameState->AccItemList[DroppedItem] > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 아이템이 최대치입니다. 드랍되지 않습니다."));
		
		GameState->AccItemCanGetList.Remove(DroppedItem);
		return;
	}

	GameState->AccItemList[DroppedItem]++;
	
	if (GameState->AccItemList[DroppedItem] > 1)
	{
		GameState->AccItemCanGetList.Remove(DroppedItem);
	}

	//ItemClass를 현재 액터 위치에 스폰시킴ㅇㅇㅇㅇㅇ
	APickupItem* SpawnedItem = GetWorld()->SpawnActor<APickupItem>(ItemClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
	
	if (DroppedItem == 0 && GameState->AccItemList[DroppedItem] == 1)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "SL";
	}
	else if (DroppedItem == 0 && GameState->AccItemList[DroppedItem] == 2)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "SL2";
	}
	else if (DroppedItem == 1 && GameState->AccItemList[DroppedItem] == 1)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "AH";
	}
	else if (DroppedItem == 1 && GameState->AccItemList[DroppedItem] == 2)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "AH2";
	}
	else if (DroppedItem == 2 && GameState->AccItemList[DroppedItem] == 1)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "EM";
	}
	else if (DroppedItem == 2 && GameState->AccItemList[DroppedItem] == 2)
	{
		SpawnedItem->ItemData->ItemDataHandle.RowName = "EM2";
	}
}


