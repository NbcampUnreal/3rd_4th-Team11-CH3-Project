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

void UHUDWidget::UpdateBossHealth(int32 CurrentHealth, int32 MaxHealth, AActor* Instigator)
{
	if (BossHealthBar)
	{
		BossHealthBar->SetPercent(1 - static_cast<float>(CurrentHealth) / MaxHealth);
		SetBossHPBarVisible(true);
	}
}

void UHUDWidget::UpdateBullet(int32 CurrentBullet)
{
	if (BulletText)
	{
		BulletText->SetText(FText::AsNumber(CurrentBullet));
	}
}

void UHUDWidget::UpdateScore(int32 CurrentScore)
{
	if (HUDScoreNum)
	{
		HUDScoreNum->SetText(FText::AsNumber(CurrentScore));
	}
}

void UHUDWidget::UpdateSubQuest(const FString& QuestText)
{
	if (SubQuestText)
	{
		SubQuestText->SetText(FText::FromString(QuestText));
	}
}

void UHUDWidget::UpdateHiddenQuest(int32 StatueCount)
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

		if (ScaleUp)
		{
			PlayAnimation(ScaleUp, 0.f, 1);
		}

		GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimerHandle);
		if (!GetWorld()->GetTimerManager().IsTimerActive(HitMarkerTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				HitMarkerTimerHandle,
				this,
				&UHUDWidget::HideHitMarker,
				0.2f,
				false
			);
		}
	}
}

void UHUDWidget::HideHitMarker()
{
	if (HitMarkerImage)
	{
		HitMarkerImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UHUDWidget::ShowKillMarker()
{
	if (KillMarker)
	{
		KillMarker->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().ClearTimer(KillMarkerTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			KillMarkerTimerHandle,
			this,
			&UHUDWidget::HideKillMarker,
			0.5f,
			false
		);
	}
}

void UHUDWidget::HideKillMarker()
{
	if (KillMarker)
	{
		KillMarker->SetVisibility(ESlateVisibility::Hidden);
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
		UE_LOG(LogTemp, Warning, TEXT("firststep!!"))
		UDamageWidget* DamageWidget = CreateWidget<UDamageWidget>(PC, DamageWidgetClass);
		if (DamageWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage!!"))
			DamageWidget->AddToViewport();
			DamageWidget->SetDamageText(DamageAmount);
			DamageWidget->SetScreenPosition(ScreenPosition);

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

void UHUDWidget::UpdateQuickSlot(FName Name, int32 Count)
{
	if (Name == "HealthPotion")
	{
		Potion->SetVisibility(ESlateVisibility::Visible);
		PotionText->SetVisibility(ESlateVisibility::Visible);
		PotionText->SetText(FText::AsNumber(Count));
	}
	else if(Name == "Adrenaline")
	{
		Adrenaline->SetVisibility(ESlateVisibility::Visible);
		AdrenalineText->SetVisibility(ESlateVisibility::Visible);
		AdrenalineText->SetText(FText::AsNumber(Count));
	}
	else if (Name == "CardKey")
	{
		CardKey->SetVisibility(ESlateVisibility::Visible);
		CardKeyText->SetVisibility(ESlateVisibility::Visible);
		CardKeyText->SetText(FText::AsNumber(Count));
	}
	else if (Name == "CatStatue")
	{
		Statue->SetVisibility(ESlateVisibility::Visible);
		StatueText->SetVisibility(ESlateVisibility::Visible);
		StatueText->SetText(FText::AsNumber(Count));
	}
}

void UHUDWidget::SetBossHPBarVisible(bool bVisible)
{
	if (BossHealthBar)
	{
		BossHealthBar->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (BossNameText)
	{
		BossNameText->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}