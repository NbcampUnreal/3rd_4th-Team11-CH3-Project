#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"
#include "HUDWidget.h"
#include "BaseStatComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStatePlay.h"

AMyPlayerController::AMyPlayerController()
{
	IMC = nullptr;
	MoveAction = nullptr;
	RunAction = nullptr;
	JumpAction = nullptr;
	CrouchAction = nullptr;
	LookAction = nullptr;
	AimAction = nullptr;
	ReloadAction = nullptr;
	ShootAction = nullptr;
	InteractionAction = nullptr;
	MyCharacter = nullptr;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC)
			{
				Subsystem->AddMappingContext(IMC, 0);
			}
		}
	}

    

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevelName.Equals(TEXT("MainMenuLevel")))
	{
		if (MainMenuWidgetClass)
		{
			UUserWidget* MainMenuWidget = CreateWidget(this, MainMenuWidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
	}
	else if (CurrentLevelName.Equals(TEXT("MainLevel")))
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			AMyCharacter* MyPlayerCharacter = Cast<AMyCharacter>(GetPawn());
			if (MyPlayerCharacter)
			{
				UBaseStatComponent* StatComponent = MyPlayerCharacter->FindComponentByClass<UBaseStatComponent>();
				if (StatComponent)
				{
					StatComponent->OnHpChangedEvent.AddDynamic(HUDWidget, &UHUDWidget::UpdateHealth);
					HUDWidget->UpdateHealth(StatComponent->GetHp(), StatComponent->GetMaxHp(), GetPawn());
				}
				AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
				if (GameStatePlay)
				{
					GameStatePlay->OnScoreChanged.AddDynamic(HUDWidget, &UHUDWidget::UpdateScore);
					HUDWidget->UpdateScore(GameStatePlay->Score);
				}
			}
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}