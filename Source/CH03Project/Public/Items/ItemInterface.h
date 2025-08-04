#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class CH03PROJECT_API IItemInterface 
{
	GENERATED_BODY()

public:
	// 상호작용 UI를 표시하는 함수
	UFUNCTION()
	virtual void ShowWidget(AActor* InteractingActor) = 0;
	// 상호작용 UI를 숨기는 함수
	UFUNCTION()
	virtual void HideWidget(AActor* InteractingActor) = 0;
	// 플레이어가 아이템을 획득하려고 할 때 호출할 함수
	UFUNCTION()
	virtual void Interact(AActor* InteractingActor) = 0;
	// 아이템을 사용하는 함수
	UFUNCTION()
	virtual void Use(ACharacter* PlayerCharacter) = 0;
};
