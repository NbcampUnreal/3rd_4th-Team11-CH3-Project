#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "CardKey.generated.h"

UCLASS()
class CH03PROJECT_API UCardKey : public UBaseItem
{
	GENERATED_BODY()
	
	virtual bool Use_Implementation(AActor* User) override;
};
