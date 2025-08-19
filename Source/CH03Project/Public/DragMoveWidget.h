#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DragMoveWidget.generated.h"


UCLASS()
class CH03PROJECT_API UDragMoveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ImageChange")
	void ImageChange(UTexture2D* NewImage, UImage* ImageWidget);
	
};
