#include "HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
	}
}

void UHUDWidget::UpdateBullet(int32 CurrentBullet)
{
	if (BulletText)
	{
		BulletText->SetText(FText::AsNumber(CurrentBullet));
	}
}

void UHUDWidget::UpdateBossHP(float CurrentBossHealth, float MaxBossHealth)
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