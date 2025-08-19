#include "EndMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UEndMenuWidget::NativeConstruct()
{
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UEndMenuWidget::OnRestartClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UEndMenuWidget::OnQuitClicked);
	}
}

void UEndMenuWidget::OnRestartClicked()
{
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UEndMenuWidget::OnQuitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UEndMenuWidget::UpdateEndScore(int32 CurrentScore)
{
	if (EndScoreNum)
	{
		EndScoreNum->SetText(FText::AsNumber(CurrentScore));
	}
}