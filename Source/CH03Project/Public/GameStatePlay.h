#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillCountChanged, int32, KillCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionTextChanged, const FString&, MissionText);


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
	
	FVector LastDeathLocation;
	void SetLastLocation(FVector LastLocation);
	FVector GetLastLocation();

	FOnKillCountChanged OnKillCountChanged;
	int32 killCount;
	void AddKillCount(int32 Points);
	int32 GetKillCount();
	

	

	FOnMissionTextChanged OnMissionTextChanged;
	FString SubMissionText;

	void SetMissionText(const FString& Text);
	FString GetMissionText() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossMaxHp;


	

};
