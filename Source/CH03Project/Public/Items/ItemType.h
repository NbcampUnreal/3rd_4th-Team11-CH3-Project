#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

// 열거형 아이템 타입 (UENUM은 uint8 타입만 지원)
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable		UMETA(DisplayName="Consumable"),
	Accessory		UMETA(DisplayName="Accessory"),
};
