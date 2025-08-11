#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "CatStatue.generated.h"

UCLASS()
class CH03PROJECT_API UCatStatue : public UBaseItem
{
	GENERATED_BODY()
	
	virtual void Use_Implementation(AActor* User) override;
};
