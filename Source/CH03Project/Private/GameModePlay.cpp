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

void AGameModePlay::AddKillCount(int32 Points)
{
	GameStatePlays->AddKillCount(Points);
}


void AGameModePlay::SetMissionText(const FString& Text)
{
	GameStatePlays->SetMissionText(Text);
}


void AGameModePlay::AddItemCount(int32 Point, int32 SlotIndex)
{
	GameStatePlays->AddItemCount(Point, SlotIndex);
	//타이머 0.1f float로
}