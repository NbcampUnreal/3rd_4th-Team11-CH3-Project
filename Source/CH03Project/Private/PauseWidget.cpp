#include "PauseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReStartButton)
		ReStartButton->OnClicked.AddDynamic(this, &UPauseWidget::OnRestartClicked);

	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitClicked);

	if (ResumeButton)
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeClicked);
}

void UPauseWidget::OnRestartClicked()
{
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void UPauseWidget::OnQuitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UPauseWidget::OnResumeClicked()
{
	RemoveFromParent();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetPause(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}