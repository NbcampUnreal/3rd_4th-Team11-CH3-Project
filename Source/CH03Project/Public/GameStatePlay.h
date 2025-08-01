#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerHpChangedSignature, int32, NewHp, int32, MaxHp, int32, OldHp);

UCLASS()
class CH03PROJECT_API AGameStatePlay : public AGameState
{
	GENERATED_BODY()


public:

	AGameStatePlay();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int PlayerHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int PlayerMaxHp;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPlayerHealth(int NewValHp, int NewValMaxHp);

	UPROPERTY(BlueprintAssignable, Category = "UI|Events")
	FOnPlayerHpChangedSignature OnPlayerHpChanged;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int Score;

	void SetScore(int NewScore);
	void AddScore(int Points);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<FString> MissionTexts;
	
	void SetMissionText(const FString& Text, int MissionIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int EnemyBossHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int EnemyBossMaxHp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<int> ItemCounts;	//퀵슬롯 수 만큼 늘려서 사용하면 될 것 같은 느낌적인 느낌?

	void AddItemCount(int Point, int SlotIndex);

};
