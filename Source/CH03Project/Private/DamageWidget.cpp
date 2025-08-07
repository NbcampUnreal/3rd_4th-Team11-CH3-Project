#include "DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UDamageWidget::SetDamageText(int32 DamageAmount)
{
    if (DamageText)
    {
        DamageText->SetText(FText::FromString(FString::FromInt(DamageAmount)));
    }
}

void UDamageWidget::SetScreenPosition(const FVector2D& ScreenPos)
{
    SetPositionInViewport(ScreenPos, true);
}