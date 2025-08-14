#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Components/Image.h"
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
	void UpdateHealth(int32 CurrentHealth, int32 MaxHealth, AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateBossHealth(int32 CurrentHealth, int32 MaxHealth, AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateBullet(int32 CurrentBullet);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateScore(int32 CurrentScore);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateQuickSlot(FName Name, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateAccessory(FName ItemID);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateSubQuest(const FString& QuestText);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateHiddenQuest(int32 StatueCount);

	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void UpdateCoolTime(float CoolTime, FName Name);

	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void ShowHitMarker();
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void ShowKillMarker();
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void ShowDamageText(int32 DamageAmount, const FVector& WorldLocation);

	UFUNCTION(BlueprintCallable)
	void SetCrosshairVisible(bool bVisible);
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void SetBossHPBarVisible(bool bVisible);

	void HideHitMarker();
	void HideKillMarker();


protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthNum;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BulletText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BossHealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HUDScoreNum;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubQuestText;
	UPROPERTY(meta = (BindWidget))
	UImage* HiddenQuestOutline;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatueNum;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HiddenQuestText;

	UPROPERTY(meta = (BindWidget))
	UImage* HitMarkerImage;
	UPROPERTY(meta = (BindWidget))
	UImage* NormalCrossHair;
	UPROPERTY(meta = (BindWidget))
	UImage* KillMarker;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PotionText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AdrenalineText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardKeyText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatueText;

	UPROPERTY(meta = (BindWidget))
	UImage* Potion;
	UPROPERTY(meta = (BindWidget))
	UImage* Adrenaline;
	UPROPERTY(meta = (BindWidget))
	UImage* CardKey;
	UPROPERTY(meta = (BindWidget))
	UImage* Statue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PotionCoolTime;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AdrenalineCoolTime;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ScaleUp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDamageWidget> DamageWidgetClass;

private:
	FTimerHandle HitMarkerTimerHandle;
	FTimerHandle KillMarkerTimerHandle;
};
