#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataRow.h"
#include "BaseItem.generated.h"

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class CH03PROJECT_API UBaseItem : public UObject
{
	GENERATED_BODY()

public:
	UBaseItem();

	// 아이템 데이터 테이블 참조 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FDataTableRowHandle ItemDataHandle;
	// 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	// 아이템 데이터 반환
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemDataRow GetItemData() const;
	// 아이템 사용
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Item")
	void Use(AActor* User);
	virtual void Use_Implementation(AActor* User);

protected:
	// 쿨타임 상태 변수
	bool bIsOnCooldown = false;

	// 쿨타임 타이머 핸들
	FTimerHandle CooldownTimerHandle;

	// 쿨타임 해제 함수
	UFUNCTION()
	virtual void ResetCooldown();
};