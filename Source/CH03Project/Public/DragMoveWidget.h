// Fill out your copyright notice in the Description page of Project Settings.

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
	//이미지의 텍스쳐 변경
	UFUNCTION(BlueprintCallable, Category = "ImageChange")
	void ImageChange(UTexture2D* NewImage, UImage* ImageWidget);
	
};
