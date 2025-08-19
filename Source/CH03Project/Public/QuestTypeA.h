#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnRow.h"
#include "GameModePlay.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Volume.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "QuestTypeA.generated.h"


class ASpawnAreaActor;
class APickupItem;
class ATriggerVolume;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestEvent);

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	int32 FirstAreaTargetKillCount;

	UFUNCTION()
	void UpdateKillCount(int32 Points);
	UFUNCTION()
	void UpdateKeyItemCount();

	//스폰데이터어레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	TSubclassOf<APickupItem> KeyItem;

	//스폰데이터어레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	TArray<UDataTable*> SpawnDataArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	UBoxComponent* QuestStartCollision;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSpawnAreas")
	TArray<ASpawnAreaActor*> SpawnLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBossAreas")
	TArray<ASpawnAreaActor*> SpawnLocationMelee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBossAreas")
	TArray<ASpawnAreaActor*> SpawnLocationRanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBossAreas")
	TSubclassOf<ACharacter> MeleeEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBossAreas")
	TSubclassOf<ACharacter> RangedEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnBossAreas")
	float SpawnDelay = 15.0f;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundWave* SoundToPlay;

	UPROPERTY(VisibleAnywhere, Category="Sound")
	class UAudioComponent* AudioComponent;

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
	void Progress07();
	void Progress08();






	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	AVolume* EnemyAliveVolume;

	UFUNCTION(BlueprintCallable)
	void DestroyAllEnemies();

	UFUNCTION(BlueprintCallable)
	void PassDoor();
	void OpenDoorCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	int32 OnDoorOpenCount;
	
	UPROPERTY(BlueprintAssignable)
	FQuestEvent OnQuestCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTypeA")
	ATriggerVolume* LastBossTextVolume;

	UFUNCTION()
	void OnLastBossTextVolumeBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);


	UFUNCTION(BlueprintCallable)
	void GameEnding();

	UFUNCTION()
	void OnEnemySpawn();


};
