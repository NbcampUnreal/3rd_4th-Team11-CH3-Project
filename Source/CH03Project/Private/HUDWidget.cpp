#include "HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GameInstance)
	//{
	//	// GameInstance의 OnHealthChanged 델리게이트에 HUDWidget의 UpdateHealth 함수 바인딩
	//	GameInstance->OnHealthChanged.AddDynamic(this, &UHUDWidget::UpdateHealth);
	//	// GameInstance의 OnBulletChanged 델리게이트에 HUDWidget의 UpdateBullet 함수 바인딩
	//	GameInstance->OnBulletChanged.AddDynamic(this, &UHUDWidget::UpdateBullet);

	//	// HUD가 생성될 때 초기 체력과 총알 값을 업데이트 (선택 사항)
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