// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlay.h"
#include "GameStatePlay.h"
#include "QuestTypeA.h"
#include "Kismet/GameplayStatics.h"
#include "BaseActor.h"

AGameModePlay::AGameModePlay()
{

}

void AGameModePlay::BeginPlay()
{
	Super::BeginPlay();

	GameStatePlays = GetGameState<AGameStatePlay>();
	
	GameStatePlays->SetScore(0);

	

}



void AGameModePlay::AddScore(int32 Points)
{
	GameStatePlays->AddScore(Points);
}


void AGameModePlay::SetMissionText(const FString& Text, int32 MissionIndex)
{
	GameStatePlays->SetMissionText(Text, MissionIndex);
	
}


void AGameModePlay::AddItemCount(int32 Point, int32 SlotIndex)
{
	GameStatePlays->AddItemCount(Point, SlotIndex);
	//타이머 0.1f float로
}

//타이머 호출 void 아이템 쿨타임 끝()

void AGameModePlay::OpenDoor()
{
	//퀘스트 문열림 체크
	if (AQuestTypeA* Quest = Cast<AQuestTypeA>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestTypeA::StaticClass())))
	{
		if (GameStatePlays->ItemCounts[4] > 0)
		{
			AddItemCount(-1, 4);
			Quest->CheckQuestDoor();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("키가 없습니다."));
		}
	}
}