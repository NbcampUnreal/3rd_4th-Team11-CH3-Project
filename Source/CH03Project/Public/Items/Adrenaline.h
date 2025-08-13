#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "Adrenaline.generated.h"

class AMyCharacter;

UCLASS()
class CH03PROJECT_API UAdrenaline : public UBaseItem
{
	GENERATED_BODY()

public:
	virtual bool Use_Implementation(AActor* User) override;

	// 공격력, 방어력 증가

};
