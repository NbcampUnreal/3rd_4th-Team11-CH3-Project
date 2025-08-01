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
	int DoorCount;

	

	FTimerHandle TimerHandle;

public:
	AQuestTypeA();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	int DoorOpenCount;//10배로 관리

	AGameModePlay* GameModePlays;
	
	//텍스트 어레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	TArray<FText> QuestTexts;
	
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
	UFUNCTION()
	void StartQuest();

	void CheckQuestDoor();

	void DoorCountCheck();

	void SpawnEnemy();

	

	

};
