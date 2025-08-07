#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnRow.generated.h"




USTRUCT(BlueprintType)
struct FEnemySpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	TSubclassOf<class AActor> EnemyClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	int SpawnCount = 0;
	

};
