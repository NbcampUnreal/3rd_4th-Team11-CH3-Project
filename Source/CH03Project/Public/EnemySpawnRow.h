#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnRow.generated.h"




USTRUCT(BlueprintType)
struct FEnemySpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	TSubclassOf<class AActor> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawn")
	int SpawnCount;
	

};
