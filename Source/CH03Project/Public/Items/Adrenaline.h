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
	UAdrenaline();
	virtual bool Use_Implementation(AActor* User) override;

	// 증가되는 공격력 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adrenaline")
	int32 IncreaseDamageStat;
	// 증가되는 방어력 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adrenaline")
	int32 IncreaseArmorStat;
	// 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adrenaline")
	float Duration;
	// 쿨타임 타이머 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Adrenaline")
	FTimerHandle DurationTimerHandle;

	// 지속지난 후 아이템 효과를 해제하는 함수
	UFUNCTION(BlueprintCallable, Category="Adrenaline")
	void ResetAdrenalineEffect(AActor* User);
};
