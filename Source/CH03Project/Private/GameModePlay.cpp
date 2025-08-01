// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlay.h"
#include "GameStatePlay.h"
#include "QuestTypeA.h"
#include "Kismet/GameplayStatics.h"

AGameModePlay::AGameModePlay()
{
}

void AGameModePlay::BeginPlay()
{
	Super::BeginPlay();

	GameStatePlays = GetGameState<AGameStatePlay>();
	
	GameStatePlays->SetScore(0);

	//컨트롤러* 컨트롤러 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	/*if (컨트롤러)
	{
		캐릭터* 캐릭터 = 컨트롤러->cast<캐릭터>(컨트롤러->GetCharacter());
		if (캐릭터)
		{
			UBaseStatComponent* BaseStatComp = 캐릭터->FindComponentByClass<UBaseStatComponent>();
			if (BaseStatComp)
			{
				BaseStatComp->OnHpChangedEvent.AddUObject(this, &AGameModePlay::HandlePlayerHpChanged);
			}
		}
	}*/
}

void AGameModePlay::HandlePlayerHpChanged(int NewHp, int MaxHp, AActor* OwnerActor)
{
	if (APawn* PlayerPawn = Cast<APawn>(OwnerActor))
	{
		if (APlayerController* PlayerController = PlayerPawn->GetController<APlayerController>())
		{
			GameStatePlays->SetPlayerHealth(NewHp, MaxHp);
		}
	}
}


void AGameModePlay::AddScore(int Points)//진짜 쓸모없다..
{
	GameStatePlays->AddScore(Points);
}


void AGameModePlay::SetMissionText(const FString& Text, int MissionIndex)
{
	GameStatePlays->SetMissionText(Text, MissionIndex);
}


void AGameModePlay::AddItemCount(int Point, int SlotIndex)
{
	GameStatePlays->AddItemCount(Point, SlotIndex);
}


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