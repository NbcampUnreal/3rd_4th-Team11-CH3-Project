#include "Items/BaseItem.h"
#include "Components/SphereComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
}

void ABaseItem::ShowWidget(AActor* InteractingActor)
{

}

void ABaseItem::HideWidget(AActor* InteractingActor)
{

}

void ABaseItem::Interact(AActor* InteractingActor)
{

}

void ABaseItem::Use(ACharacter* PlayerCharacter)
{

}

void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

}

void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

}