#include "MenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitClicked);
}

void UMenuWidget::OnStartClicked()
{
    RemoveFromParent();
    UGameplayStatics::OpenLevel(GetWorld(), FName("OpeningLevel"));
}

void UMenuWidget::OnQuitClicked()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UMenuWidget::SetStartButtonText(bool bIsGameOver)
{
    if (StartButtonText)
    {
        FString Label = bIsGameOver ? TEXT("RESTART") : TEXT("Start");
        StartButtonText->SetText(FText::FromString(Label));
    }
}

void UMenuWidget::SetTileText(bool bIsGameOver)
{
    if (FPSGameText)
    {
        FString Label = bIsGameOver ? TEXT("Game Over!!") : TEXT("FPS Game");
        FPSGameText->SetText(FText::FromString(Label));
    }
}

void UMenuWidget::SetScoreNumText(bool bIsGameOver, int Score)
{
    if (bIsGameOver)
    {
        if (ScoreNumText)
        {
            ScoreNumText->SetVisibility(ESlateVisibility::Visible);
            ScoreNumText->SetText(FText::AsNumber(Score));
        }
    }
    else
    {
        if (ScoreNumText)
        {
            ScoreNumText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UMenuWidget::SetScoreText(bool bIsGameOver)
{
    if (bIsGameOver)
    {
        if (ScoreText)
        {
            ScoreText->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if (ScoreText)
        {
            ScoreText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UMenuWidget::SetMenuState(bool bIsGameOver, int score)
{
    SetStartButtonText(bIsGameOver);
    SetTileText(bIsGameOver);
    SetScoreNumText(bIsGameOver, score);
    SetScoreText(bIsGameOver);
}