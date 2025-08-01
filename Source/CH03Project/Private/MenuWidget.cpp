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
    UGameplayStatics::OpenLevel(GetWorld(), FName("GameMap"));
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