#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CH03PROJECT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

public:
	UFUNCTION(BlueprintCallable)
	void SetStartButtonText(bool bIsGameOver);

	UFUNCTION(BlueprintCallable)
	void SetTileText(bool bIsGameOver);

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StartButtonText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FPSGameText;
};
