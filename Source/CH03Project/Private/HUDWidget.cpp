#include "HUDWidget.h"
#include "DamageWidget.h"
#include "MyPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"



void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
void UHUDWidget::UpdateHealth(int32 CurrentHealth, int32 MaxHealth, AActor* Instigator)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(1-static_cast<float>(CurrentHealth) / MaxHealth);
	}

	if (HealthNum)
	{
		FString HealthString = FString::Printf(TEXT("%d"), CurrentHealth);
		HealthNum->SetText(FText::FromString(HealthString));
	}
}


void UHUDWidget::UpdateBullet(int32 CurrentBullet)
{
	if (BulletText)
	{
		BulletText->SetText(FText::AsNumber(CurrentBullet));
	}
}

void UHUDWidget::UpdateBossHP(int CurrentBossHealth, int MaxBossHealth)
{
	if (BossHealthBar)
	{
		BossHealthBar->SetPercent(CurrentBossHealth / MaxBossHealth);
	}
}

void UHUDWidget::UpdateScore(int32 CurrentScore)
{
	if (HUDScoreNum)
	{
		HUDScoreNum->SetText(FText::AsNumber(CurrentScore));
	}
}

void UHUDWidget::UpdateSubQuest(int32 QuestIndex, const TArray<FString>& MissionTexts)
{
	if (SubQuestText && MissionTexts.IsValidIndex(QuestIndex))
	{
		FString QuestText = MissionTexts[QuestIndex];
		SubQuestText->SetText(FText::FromString(QuestText));
	}
}

void UHUDWidget::UpdateHiddenQuest(bool bIsGetStatue, int32 StatueCount)
{
	if (HiddenQuestText)
	{
		HiddenQuestText->SetVisibility(ESlateVisibility::Visible);
	}

	if (StatueNum)
	{
		StatueNum->SetVisibility(ESlateVisibility::Visible);
		StatueNum->SetText(FText::AsNumber(StatueCount));
	}

	if (HiddenQuestOutline)
	{
		HiddenQuestOutline->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDWidget::ShowHitMarker()
{
	if (HitMarkerImage)
	{
		HitMarkerImage->SetVisibility(ESlateVisibility::Visible);

		GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			HitMarkerTimerHandle,
			this,
			&UHUDWidget::HideHitMarker,
			0.3f,
			false
		);
	}
}

void UHUDWidget::HideHitMarker()
{
	if (HitMarkerImage)
	{
		HitMarkerImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::ShowDamageText(int32 DamageAmount, const FVector& WorldLocation)
{
	if (!DamageWidgetClass) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	FVector2D ScreenPosition;
	if (PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition))
	{
		UDamageWidget* DamageWidget = CreateWidget<UDamageWidget>(PC, DamageWidgetClass);
		if (DamageWidget)
		{
			DamageWidget->AddToViewport();
			DamageWidget->SetDamageText(DamageAmount);
			DamageWidget->SetScreenPosition(ScreenPosition);

			// 제거 타이머
			FTimerHandle RemoveTimer;
			FTimerDelegate RemoveDelegate = FTimerDelegate::CreateLambda([DamageWidget]()
				{
					if (DamageWidget)
					{
						DamageWidget->RemoveFromParent();
					}
				});

			PC->GetWorldTimerManager().SetTimer(RemoveTimer, RemoveDelegate, 1.5f, false);
		}
	}
}

void UHUDWidget::SetCrosshairVisible(bool bVisible)
{
	if (NormalCrossHair)
	{
		NormalCrossHair->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}