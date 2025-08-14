#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/PickupItem.h"
#include "AccDropComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UAccDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UAccDropComponent();

	void DeathEnemy();
protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	int32 ChanceToDrop = 0;

	void DropItem();

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<APickupItem> ItemClass;
	

};
