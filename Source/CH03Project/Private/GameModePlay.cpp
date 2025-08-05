// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlay.h"
#include "GameStatePlay.h"
#include "QuestTypeA.h"
#include "Kismet/GameplayStatics.h"
#include "BaseActor.h"
#include "BaseStatComponent.h"
#include "Blueprint/UserWidget.h"

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

	//테스트용
	ABaseActor* BaseActor = Cast<ABaseActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseActor::StaticClass()));
	if (BaseActor)
	{
		UBaseStatComponent* BaseStatComp = BaseActor->FindComponentByClass<UBaseStatComponent>();
		if (BaseStatComp)
		{
			BaseStatComp->OnHpChangedEvent.AddDynamic(this, &AGameModePlay::HandlePlayerHpChanged);
		}
	}

	if (MainMenuWidgetClass)
	{
		UUserWidget* MainMenuWidget = CreateWidget(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}

void AGameModePlay::HandlePlayerHpChanged(int32 NewHp, int32 MaxHp, AActor* OwnerActor)
{
	if (APawn* PlayerPawn = Cast<APawn>(OwnerActor))
	{
		if (APlayerController* PlayerController = PlayerPawn->GetController<APlayerController>())
		{
			GameStatePlays->SetPlayerHealth(NewHp, MaxHp);

			//HUDWidget->UpdateHealth(NewHp, MaxHp);
		}
	}
}


void AGameModePlay::AddScore(int32 Points)//진짜 쓸모없다..
{
	GameStatePlays->AddScore(Points);
	//HUDWidget->UpdateScore(GameStatePlays->Score);
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