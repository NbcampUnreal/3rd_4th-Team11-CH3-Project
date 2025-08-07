#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "Adrenaline.generated.h"

UCLASS()
class CH03PROJECT_API AAdrenaline : public ABaseItem
{
	GENERATED_BODY()
	
public:

	// 공격 속도?장전 속도?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float ReloadSpeedMultiplier;
	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float MovementSpeedMultiplier;
	// 효과 지속 시간

};
