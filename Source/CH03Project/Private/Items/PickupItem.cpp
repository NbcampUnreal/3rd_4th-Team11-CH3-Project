#include "Items/PickupItem.h"
#include "Items/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

APickupItem::APickupItem()
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
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnItemEndOverlap);

	// 상호작용 UI
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(Scene);
	InteractUI->SetWidgetSpace(EWidgetSpace::Screen);
	InteractUI->SetVisibility(false);
}

// 아이템마다의 점수를 반환하는 함수
int32 APickupItem::GetItemScore() const
{
	if(ItemData)
	{
		return ItemData->GetItemData().Score;
	}
	return 0;
}

// 오버랩 됐을 때 아이템 위에 상호작용 UI를 띄울 함수
void APickupItem::ShowWidget()
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(true);
	}
}

// 오버랩이 끝났을 때 아이템 위에 상호작용 UI를 숨길 함수
void APickupItem::HideWidget()
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(false);
	}
}

// 아이템과 오버랩 상태에서 상호작용 E키를 누를때 해당 함수를 호출함
// 해당 함수를 호출하면 InventoryComponent (ActorComponent) 에 아이템을 저장하는 함수
void APickupItem::Interact()
{
	if(!ItemData)
		return;

	// 플레이어 찾기
	AActor* PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(!PlayerActor)
		return;

	// InventoryComponent 가져오기
	auto InventoryComp = PlayerActor->FindComponentByClass<UInventoryComponent>();
	if(!InventoryComp)
		return;

	// 인벤토리에 추가
	InventoryComp->AddItem(ItemData);

	// 월드에서 제거
	Destroy();
}

// 오버랩 됐을 때 위젯을 표시
void APickupItem::OnItemOverlap(
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
		Interact();
	}
}

// 오버랩이 끝났을 때 위젯을 숨김
void APickupItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	HideWidget();
}