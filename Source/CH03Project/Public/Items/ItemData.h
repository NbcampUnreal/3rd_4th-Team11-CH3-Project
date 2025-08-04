#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

// 열거형 아이템 타입 (UENUM은 uint8 타입만 지원)
UENUM(BlueprintType)
enum class EItemType : uint8
{
	HealthPotion,
	Adrenaline,
	CatStatue
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemID; // 데이터 테이블의 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Name; // 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Description; // 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftObjectPtr<UTexture2D> Icon; // 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftObjectPtr<USoundBase> PickupSound; // 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSoftObjectPtr<UParticleSystem> PickupEffect; // 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsStackable; // 겹칠 수 있는 아이템인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 MaxStackSize; // 최대 수량
};