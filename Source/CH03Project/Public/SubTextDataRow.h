#pragma once

#include "CoreMinimal.h"
#include "SubTextDataRow.generated.h"

USTRUCT(BlueprintType)
struct FSubTextDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubText")
	FString SubText;


};
