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


void UDamageComponent::TransDamage(AActor* TargetActor)
{
	if (TargetActor)
	{
		UBaseStatComponent* TargetStatComponent = TargetActor->FindComponentByClass<UBaseStatComponent>();
		if (TargetStatComponent)
		{
			TargetStatComponent->AddHp(-AttackDamage);
		}
	}
}

bool UDamageComponent::StoreAttackToken(int32 Amount)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		return true;
	}
	return false;
}

void UDamageComponent::ReturnAttackToken(int32 Amount)
{
	AttackTokenCount += Amount;
}
