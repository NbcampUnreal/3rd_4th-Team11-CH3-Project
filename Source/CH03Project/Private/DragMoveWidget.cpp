#include "DragMoveWidget.h"


void UDragMoveWidget::ImageChange(UTexture2D* NewImage, UImage* ImageWidget)
{
	if (ImageWidget && NewImage)
	{
		ImageWidget->SetBrushFromTexture(NewImage);
	}
}
