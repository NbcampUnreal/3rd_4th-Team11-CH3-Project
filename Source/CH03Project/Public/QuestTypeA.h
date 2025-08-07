#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnRow.h"
#include "GameModePlay.h"
#include "Components/BoxComponent.h"
#include "QuestTypeA.generated.h"


UCLASS()
class CH03PROJECT_API AQuestTypeA : public AActor
{
	GENERATED_BODY()
	
private:
	
	FTimerHandle TimerHandle;

public:
	AQuestTypeA();

protected:
	virtual void BeginPlay() override;

public:
	int32 ProgressStage;
	
	AGameModePlay* GameModePlays;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	UDataTable* SubTextsDataTable;
	TArray<FString> SubTexts;
	void SetSubTexts();
	

	int32 StartKillCount;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	//int32 FirstAreaEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	int32 FirstAreaTargetKillCount;

	UFUNCTION()
	void UpdateKillCount(int32 Points);
	UFUNCTION()
	void UpdateKeyItemCount(int32 Points);

	//스폰데이터어레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	TArray<UDataTable*> SpawnDataArray;




	//--------------------------------- 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	UBoxComponent* QuestStartCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	TArray<UBoxComponent*> SpawnLocation;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ProgressStarter();
	void Progress00();
	void Progress01();
	void Progress02();
	void Progress03();
	void Progress04();
	void Progress05();
	void Progress06();







	
	

	void SpawnEnemy();

	

	

};
