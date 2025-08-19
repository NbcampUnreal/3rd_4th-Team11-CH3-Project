

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnAreaActor.generated.h"


UCLASS()
class CH03PROJECT_API ASpawnAreaActor : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawnAreaActor();

protected:

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* SpawnArea;

};
