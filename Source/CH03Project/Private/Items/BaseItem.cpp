#include "Items/BaseItem.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

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
	if(!ItemDataHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataHandle is null")); // 디버깅용
		return 0;
	}

	const FItemDataRow* ItemRow = ItemDataHandle.GetRow<FItemDataRow>("GetItemScore");

	if(ItemRow)
	{
		UE_LOG(LogTemp, Log, TEXT("Item Score: %d"), ItemRow->Score);
		return ItemRow->Score;
	}

	return 0;
}

// 오버랩 됐을 때 아이템 위에 상호작용 UI를 띄울 함수
void ABaseItem::ShowWidget(AActor* InteractingActor)
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(true);
	}
}

// 오버랩이 끝났을 때 아이템 위에 상호작용 UI를 숨길 함수
void ABaseItem::HideWidget(AActor* InteractingActor)
{
	if(InteractUI)
	{
		InteractUI->SetVisibility(false);
	}
}

// 오버랩 상태일 때 E키를 누를 시 아이템을 퀵슬롯으로 들어오게 할 함수
// 최대 스택 여부 확인해야함
// 스택 가능한 아이템인지 확인해야함
// 액터 소멸 처리 해야함
void ABaseItem::Interact(AActor* InteractingActor)
{

}

// Player가 아이템을 가지고 있을 때 퀵슬롯 번호를 눌러 사용하는 함수
void ABaseItem::Use(ACharacter* PlayerCharacter)
{

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
		ShowWidget(OtherActor);
	}
}

// 오버랩이 끝났을 때 위젯을 숨김
void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	HideWidget(OtherActor);
}