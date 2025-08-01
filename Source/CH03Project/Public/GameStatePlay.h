#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerHpChangedSignature, int32, NewHp, int32, MaxHp, int32, OldHp);

UCLASS()
class CH03PROJECT_API AGameStatePlay : public AGameState
{
	GENERATED_BODY()


public:

	AGameStatePlay();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 PlayerHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 PlayerMaxHp;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPlayerHealth(int32 NewValHp, int32 NewValMaxHp);

	//UPROPERTY(BlueprintAssignable, Category = "UI|Events")
	//FOnPlayerHpChangedSignature OnPlayerHpChanged;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 Score;

	void SetScore(int32 NewScore);
	void AddScore(int32 Points);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<FString> MissionTexts; //0 서브 1 히든

	void SetMissionText(const FString& Text, int32 MissionIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 EnemyBossMaxHp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<int32> ItemCounts;	//0 힐템 1 아드 2 키 3 고양이


	void AddItemCount(int32 Point, int32 SlotIndex);

};
