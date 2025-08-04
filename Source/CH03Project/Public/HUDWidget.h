#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

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
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateScore(int32 CurrentScore);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateSubQuest(int32 QuestIndex, const TArray<FString>& MissionTexts);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateHiddenQuest(bool bIsGetStatue, int32 StatueCount);



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
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HUDScoreNum;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubQuestText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestNumText;
	UPROPERTY(meta = (BindWidget))
	UImage* HiddenQuestOutline;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatueNum;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HiddenQuestText;
};
