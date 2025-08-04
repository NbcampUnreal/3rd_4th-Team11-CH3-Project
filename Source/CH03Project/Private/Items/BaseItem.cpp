#include "Items/BaseItem.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

