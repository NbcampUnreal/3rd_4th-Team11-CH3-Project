#include "DamageComponent.h"


UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UDamageComponent::TransDamage(UBaseStatComponent* TargetStatComponent)
{
	if (TargetStatComponent)
	{
		TargetStatComponent->AddHp(-AttackDamage);
	}
}
