// Fill out your copyright notice in the Description page of Project Settings.


#include "DragMoveWidget.h"


void UDragMoveWidget::ImageChange(UTexture2D* NewImage, UImage* ImageWidget)
{
	if (ImageWidget && NewImage)
	{
		ImageWidget->SetBrushFromTexture(NewImage);
	}
}
