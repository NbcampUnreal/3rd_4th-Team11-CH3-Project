#include "DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UDamageWidget::SetDamageText(int32 Damage)
{
    if (DamageText)
    {
        DamageText->SetText(FText::FromString(FString::FromInt(Damage)));
    }
}

void UDamageWidget::SetScreenPosition(const FVector2D& ScreenPos)
{
    SetPositionInViewport(ScreenPos, true);
}