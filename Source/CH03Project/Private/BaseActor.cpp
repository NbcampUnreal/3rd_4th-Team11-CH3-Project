#include "BaseActor.h"
#include "DamageComponent.h"
#include "BaseStatComponent.h"

ABaseActor::ABaseActor()
{	
	PrimaryActorTick.bCanEverTick = true;

	BaseStatComponent = CreateDefaultSubobject<UBaseStatComponent>(TEXT("StatComponent"));
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));
	DamageComponent->AttackTokenCount = 1;
}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

