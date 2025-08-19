// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlay.h"
#include "GameStatePlay.h"
#include "QuestTypeA.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BaseStatComponent.h"
#include "BaseEnemy.h"
#include "Delegates/DelegateCombinations.h"

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

void AGameModePlay::SetLastLocation(FVector LastLocation)
{
	GameStatePlays->SetLastLocation(LastLocation);
}

void AGameModePlay::SetMissionText(FString Text)
{	
	GameStatePlays->SetMissionText(Text);
}


void AGameModePlay::SetGameStatePlay()
{
	// 게임 상태를 Play로 설정
	if (GameStatePlays == nullptr)
	{
		GameStatePlays = GetGameState<AGameStatePlay>();
	}
}

void AGameModePlay::ClearEnemiesOnPlayerDeath()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Enemies);

	for (AActor* Enemy : Enemies)
	{
		if (Enemy)
		{
			Enemy->Destroy();
		}
	}
}
