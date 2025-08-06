// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatePlay.h"


AGameStatePlay::AGameStatePlay()
{

}


void AGameStatePlay::SetScore(int32 NewScore)
{
    Score = NewScore;
}

void AGameStatePlay::AddScore(int32 Points)
{
    Score += Points;
}





void AGameStatePlay::SetMissionText(const FString& Text, int32 MissionIndex)
{
    if (MissionIndex >= 0 && MissionIndex < MissionTexts.Num())
    {
        MissionTexts[MissionIndex] = Text;
    }
}





void AGameStatePlay::AddItemCount(int32 Point, int32 SlotIndex)
{
    if (ItemCounts.Num() > SlotIndex)
    {
		ItemCounts[SlotIndex] = FMath::Max(ItemCounts[SlotIndex] + Point, 0);
    }
}