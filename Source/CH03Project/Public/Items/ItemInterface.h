#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IItemInterface
{
	GENERATED_BODY()

public:
	// 플레이어가 아이템을 획득하려고 할 때 호출할 함수
	UFUNCTION()
	virtual void Interact() = 0;
};