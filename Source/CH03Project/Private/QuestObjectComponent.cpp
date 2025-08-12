#include "QuestObjectComponent.h"


UQuestObjectComponent::UQuestObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UQuestObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UQuestObjectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		
	}
}

