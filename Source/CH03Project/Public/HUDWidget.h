#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class CH03PROJECT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateBullet(int32 CurrentBullet);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateBossHP(float CurrentBossHealth, float MaxBossHealth);


protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BulletText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BossHealthBar;

};
