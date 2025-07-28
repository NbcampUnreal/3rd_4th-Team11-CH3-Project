#include "BaseActor.h"

ABaseActor::ABaseActor()
{	
	PrimaryActorTick.bCanEverTick = true;
	BaseStatComponent = CreateDefaultSubobject<UBaseStatComponent>(TEXT("StatComponent"));
}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

