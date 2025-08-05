#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemType.h"
#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY();

public:
	// 데이터 테이블의 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText ItemID;
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemType ItemType;
	// 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText ItemName;
	// 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText Description;
	// 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSoftObjectPtr<UTexture2D> Icon;
	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSoftObjectPtr<USoundBase> PickupSound;
	// 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSoftObjectPtr<UParticleSystem> PickupEffect;
	// 겹칠 수 있는 아이템인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	bool bIsStackable;
	// 최대 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxStackSize;
	// 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 Score;
	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	float CooldownTime;
};