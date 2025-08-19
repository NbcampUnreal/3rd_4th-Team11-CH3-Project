#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "MoveTriggerArea.generated.h"

class ATriggerVolume;

UCLASS()
class CH03PROJECT_API AMoveTriggerArea : public AActor
{
	GENERATED_BODY()
	
public:
	AMoveTriggerArea();

protected:	
	virtual void BeginPlay() override;
public:	
	UFUNCTION()
	void OnVolumeBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyComing")
	ATriggerVolume* EnemyTargetingVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyComing")
	ATriggerVolume* EnemyStartingVolume;

};
