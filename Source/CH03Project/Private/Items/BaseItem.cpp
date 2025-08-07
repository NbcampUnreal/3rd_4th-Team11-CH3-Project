#include "Items/BaseItem.h"
#include "Items/ItemDataRow.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameStatePlay.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

	// 상호작용 UI
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(Scene);
	InteractUI->SetWidgetSpace(EWidgetSpace::Screen);
	InteractUI->SetVisibility(false);
}

// 아이템마다의 점수를 반환하는 함수
int32 ABaseItem::GetItemScore() const
{
	// 데이터 테이블 핸들이 유효한지 확인
	if(!ItemDataHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataHandle is Null"));
		return 0;
	}

	const FItemDataRow* ItemRow = ItemDataHandle.GetRow<FItemDataRow>("GetItemScore");

	// FItemDataRow를 정상적으로 오는지 확인
	if(ItemRow)
	{
		UE_LOG(LogTemp, Log, TEXT("Item Score: %d"), ItemRow->Score);
		return ItemRow->Score;
	}

	return 0;
}

// 오버랩 됐을 때 아이템 위에 상호작용 UI를 띄울 함수
void ABaseItem::ShowWidget()
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(true);
	}
}

// 오버랩이 끝났을 때 아이템 위에 상호작용 UI를 숨길 함수
void ABaseItem::HideWidget()
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(false);
	}
}

// 아이템과 오버랩 상태에서 E키를 누를때 해당 함수를 호출함
void ABaseItem::Interact()
{
	// GameState 가져오기
	AGameStatePlay* GameState = GetWorld()->GetGameState<AGameStatePlay>();

	// 데이터 테이블 핸들이 유효한지 확인
	if (!ItemDataHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataHandle is Null"));
		Destroy(); // 아이템 데이터를 찾을 수 없어도 상호작용은 완료된 것으로 처리
		return;
	}

	// 데이터 테이블에서 아이템 정보 가져오기
	const FItemDataRow* ItemRow = ItemDataHandle.GetRow<FItemDataRow>("Interact");

	// GameState와 ItemRow가 모두 유효한지 확인
	if (GameState && ItemRow)
	{
		// ItemCounts 배열의 유효한 인덱스인지 확인
		if (GameState->ItemCounts.IsValidIndex(ItemRow->ItemID))
		{
			// 해당 아이템 ID의 카운트 증가
			GameState->ItemCounts[ItemRow->ItemID]++;
			UE_LOG(LogTemp, Log, TEXT("Item ID: %d count is now %d"), ItemRow->ItemID, GameState->ItemCounts[ItemRow->ItemID]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item ID %d is not a valid index in ItemCounts array."), ItemRow->ItemID);
		}
	}
	else
	{
		if(!GameState) UE_LOG(LogTemp, Warning, TEXT("GameStatePlay is not valid!"));
		if(!ItemRow) UE_LOG(LogTemp, Warning, TEXT("ItemRow is not valid!"));
	}

	// 상호작용이 끝났으므로 UI를 숨기고 객체를 파괴
	HideWidget();
	Destroy();
}

// 오버랩 됐을 때 위젯을 표시
void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("overlap")));
		ShowWidget();
	}
}

// 오버랩이 끝났을 때 위젯을 숨김
void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	HideWidget();
}