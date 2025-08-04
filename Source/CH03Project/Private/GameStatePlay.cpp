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


void AGameStatePlay::SetPlayerHealth(int32 NewValHp, int32 NewValMaxHp)
{
    int32 OldHpBeforeChange = PlayerHp;

    //PlayerHp = FMath::Clamp(NewValHp, 0, NewValMaxHp);
    PlayerHp = NewValHp;
    PlayerMaxHp = NewValMaxHp;

    //OnPlayerHpChanged.Broadcast(PlayerHp, PlayerMaxHp, OldHpBeforeChange);
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