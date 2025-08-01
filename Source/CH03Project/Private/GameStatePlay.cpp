// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatePlay.h"


AGameStatePlay::AGameStatePlay()
{

}


void AGameStatePlay::SetScore(int NewScore)
{
    Score = NewScore;
}

void AGameStatePlay::AddScore(int Points)
{
    Score += Points;
}


void AGameStatePlay::SetPlayerHealth(int NewValHp, int NewValMaxHp)
{
    int OldHpBeforeChange = PlayerHp;

    //PlayerHp = FMath::Clamp(NewValHp, 0, NewValMaxHp);
    PlayerHp = NewValHp;
    PlayerMaxHp = NewValMaxHp;

    OnPlayerHpChanged.Broadcast(PlayerHp, PlayerMaxHp, OldHpBeforeChange);
}



void AGameStatePlay::SetMissionText(const FString& Text, int MissionIndex)
{
    if (MissionIndex >= 0 && MissionIndex < MissionTexts.Num())
    {
        MissionTexts[MissionIndex] = Text;
    }
}





void AGameStatePlay::AddItemCount(int Point, int SlotIndex)
{
    if (ItemCounts.Num() > SlotIndex)
    {
		ItemCounts[SlotIndex] = FMath::Max(ItemCounts[SlotIndex] + Point, 0);
    }
}