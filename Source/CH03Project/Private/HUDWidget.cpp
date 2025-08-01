#include "HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GameInstance)
	//{
	//	// GameInstance�� OnHealthChanged ��������Ʈ�� HUDWidget�� UpdateHealth �Լ� ���ε�
	//	GameInstance->OnHealthChanged.AddDynamic(this, &UHUDWidget::UpdateHealth);
	//	// GameInstance�� OnBulletChanged ��������Ʈ�� HUDWidget�� UpdateBullet �Լ� ���ε�
	//	GameInstance->OnBulletChanged.AddDynamic(this, &UHUDWidget::UpdateBullet);

	//	// HUD�� ������ �� �ʱ� ü�°� �Ѿ� ���� ������Ʈ (���� ����)
	//	UpdateHealth(GameInstance->CurrentPlayerHealth, GameInstance->MaxPlayerHealth);
	//	UpdateBullet(GameInstance->CurrentPlayerBullet);
	//}
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