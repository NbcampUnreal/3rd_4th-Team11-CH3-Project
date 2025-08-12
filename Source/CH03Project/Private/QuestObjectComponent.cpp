#include "QuestObjectComponent.h"
#include "QuestTypeA.h"
#include "Kismet/GameplayStatics.h"

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
		UE_LOG(LogTemp, Warning, TEXT("퀘스트 오브젝트 컴포넌트가 파괴되었습니다."));
		AQuestTypeA* QuestActor = Cast<AQuestTypeA>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestTypeA::StaticClass()));
		if (QuestActor)
		{
			QuestActor->UpdateKeyItemCount();
			UE_LOG(LogTemp, Warning, TEXT("퀘스트 오브젝트가 파괴되었습니다: %s"), *QuestActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("퀘스트 오브젝트 컴포넌트가 소속된 액터를 찾을 수 없습니다."));
		}
	}
}

