#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillCountChanged, int32, KillCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionTextChanged, const FString&, MissionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyItemChanged, int32, KeyItem);

UCLASS()
class CH03PROJECT_API AGameStatePlay : public AGameState
{
	GENERATED_BODY()


public:

	AGameStatePlay();

	FOnScoreChanged OnScoreChanged;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 Score;

	void SetScore(int32 NewScore);
	void AddScore(int32 Points);
	
	FOnKillCountChanged OnKillCountChanged;
	int32 killCount;
	void AddKillCount(int32 Points);
	int32 GetKillCount();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 CurrentAmmo;

	FOnMissionTextChanged OnMissionTextChanged;
	FString SubMissionText;

	void SetMissionText(const FString& Text);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossMaxHp;


	FOnKeyItemChanged OnKeyItemChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<int32> ItemCounts;	//0 힐템 1 아드 2 키 3 고양이


	void AddItemCount(int32 Point, int32 SlotIndex);

};
