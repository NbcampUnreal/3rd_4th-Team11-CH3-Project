#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HealthPotion.generated.h"

UCLASS()
class CH03PROJECT_API UHealthPotion : public UBaseItem
{
	GENERATED_BODY()
	
public:
	UHealthPotion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthPotion")
	float HealAmount;

	virtual bool Use_Implementation(AActor* User) override;
};