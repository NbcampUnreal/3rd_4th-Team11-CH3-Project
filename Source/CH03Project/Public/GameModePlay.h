// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModePlay.generated.h"

class UBaseStatComponent;
class AGameStatePlay;
//class AMyCharacter; //캐릭터도 전방선언


UCLASS()
class CH03PROJECT_API AGameModePlay : public AGameMode
{
	GENERATED_BODY()



public:
	AGameModePlay();
	
protected:
	void BeginPlay();

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetLastLocation(FVector LastLocation);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddKillCount(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetMissionText(FString Text);



	void SetGameStatePlay();
	

private:
	
	
	AGameStatePlay* GameStatePlays;

public:

	void ClearEnemiesOnPlayerDeath();
};
