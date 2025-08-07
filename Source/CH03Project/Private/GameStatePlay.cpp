// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatePlay.h"


AGameStatePlay::AGameStatePlay()
{
    //초기화
	Score = 0;
    killCount = 0;
	ItemCounts = TArray<int32>({ 0, 0, 0, 0 }); // 힐템, 아드, 키, 고양이
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
	OnMissionTextChanged.Broadcast(SubMissionText);
}





void AGameStatePlay::AddItemCount(int32 Point, int32 SlotIndex)
{
    if (ItemCounts.Num() > SlotIndex)
    {
		ItemCounts[SlotIndex] = FMath::Max(ItemCounts[SlotIndex] + Point, 0);

        if (SlotIndex == 2)
        {
			OnKeyItemChanged.Broadcast(ItemCounts[SlotIndex]);
        }
    }
}