#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CH03PROJECT_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	
	
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnResumeClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* ReStartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PauseScore;

public:
	UFUNCTION()
	void UpdateScore(int32 CurrentScore);
};
