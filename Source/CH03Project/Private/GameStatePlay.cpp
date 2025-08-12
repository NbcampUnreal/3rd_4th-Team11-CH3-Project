// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatePlay.h"


AGameStatePlay::AGameStatePlay()
{
    //초기화
	Score = 0;
    killCount = 0;
    SubMissionText = TEXT("");
}


void AGameStatePlay::SetScore(int32 NewScore)
{
    Score = NewScore;
	OnScoreChanged.Broadcast(Score);
}

void AGameStatePlay::AddScore(int32 Points)
{
    Score += Points;
    OnScoreChanged.Broadcast(Score);
}

void AGameStatePlay::SetLastLocation(FVector LastLocation)
{
    LastDeathLocation = LastLocation;
}

FVector AGameStatePlay::GetLastLocation()
{
    return LastDeathLocation;
}

void AGameStatePlay::AddKillCount(int32 Points)
{
    killCount += Points;
	OnKillCountChanged.Broadcast(killCount);
}

int AGameStatePlay::GetKillCount()
{
    return killCount;
}


void AGameStatePlay::SetMissionText(const FString& Text)
{	
    SubMissionText = Text;
	UE_LOG(LogTemp, Warning, TEXT("SubMissionText: %s"), *SubMissionText);
	OnMissionTextChanged.Broadcast(SubMissionText);
}
FString AGameStatePlay::GetMissionText() const
{
    return SubMissionText;
}



