#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HealthPotion.generated.h"

UCLASS()
class CH03PROJECT_API AHealthPotion : public ABaseItem
{
	GENERATED_BODY()
	
public:

	// 힐량을 50%
	// 힐량 = [전체 체력] / 0.5
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 HealAmount;
};
