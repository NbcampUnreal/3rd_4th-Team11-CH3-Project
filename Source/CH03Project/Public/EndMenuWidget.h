// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndMenuWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CH03PROJECT_API UEndMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct();

public:
	UFUNCTION()
	void OnQuitClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void UpdateEndScore(int32 CurrentScore);

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EndScoreNum;
};
