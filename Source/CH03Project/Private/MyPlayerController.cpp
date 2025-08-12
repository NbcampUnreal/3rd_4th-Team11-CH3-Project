#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyCharacter.h"
#include "HUDWidget.h"
#include "BaseStatComponent.h"
#include "PauseWidget.h"
#include "Items/InventoryComponent.h"
#include "Items/BaseItem.h" 
#include "BaseRangedWeapon.h"
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
	PauseAction = nullptr;
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

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseAction)
		{
			EnhancedInputComp->BindAction(PauseAction, ETriggerEvent::Started, this, &AMyPlayerController::OnPauseMenu);
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
					GameStatePlay->OnMissionTextChanged.AddDynamic(HUDWidget, &UHUDWidget::UpdateSubQuest);
					HUDWidget->UpdateSubQuest(GameStatePlay->GetMissionText());
					//GameStatePlay->OnHiddenItemChanged.AddDynamic(HUDWidget, &UHUDWidget::UpdateHiddenQuest);
				}

				if (UInventoryComponent* QuickSlot = MyPlayerCharacter->FindComponentByClass<UInventoryComponent>())
				{
					QuickSlot->OnAddItemChanged.AddDynamic(this, &AMyPlayerController::HandleAddItemChanged);
					QuickSlot->OnRemoveItemChanged.AddDynamic(this, &AMyPlayerController::HandleRemoveItemChanged);
				}

				MyPlayerCharacter->OnChangedIsAiming.AddDynamic(HUDWidget, &UHUDWidget::SetCrosshairVisible);
			}
		}

		TArray<AActor*> BossActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Boss"), BossActors);
		if (BossActors.Num() > 0)
		{
			if (UBaseStatComponent* BossStat = BossActors[0]->FindComponentByClass<UBaseStatComponent>())
			{
				// 1) HUD 갱신 바인딩
				BossStat->OnHpChangedEvent.AddDynamic(HUDWidget, &UHUDWidget::UpdateBossHealth);

				// 2) 처음 데미지 때만 바 보이기
				BossStat->OnHpChangedEvent.AddDynamic(this, &AMyPlayerController::HandleBossHpChanged);
			}
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}


void AMyPlayerController::BindDeligateToSpawnedWeapon(AActor* SpawnedWeapon)
{
	if (SpawnedWeapon)
	{
		ABaseRangedWeapon* Weapon = Cast<ABaseRangedWeapon>(SpawnedWeapon);
		if (Weapon)
		{
			if (HUDWidget)
			{
				Weapon->OnChangeCurrentAmmo.AddDynamic(HUDWidget, &UHUDWidget::UpdateBullet);
			}
		}
	}
}

void AMyPlayerController::HandleAddItemChanged(FName ItemID, int32 Quantity)
{
	if (HUDWidget) { HUDWidget->UpdateQuickSlot(ItemID, Quantity); }
}

void AMyPlayerController::HandleRemoveItemChanged(FName ItemID, int32 Quantity)
{
	if (HUDWidget) { HUDWidget->UpdateQuickSlot(ItemID, Quantity); }
}

void AMyPlayerController::OnPauseMenu()
{
	PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
	if (PauseWidget)
	{
		AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
		if (GameStatePlay)
		{
			PauseWidget->UpdateScore(GameStatePlay->Score);	
		}
	}
	PauseWidget->AddToViewport();
	SetPause(true);
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void AMyPlayerController::HandleBossHpChanged(int32 Current, int32 Max, AActor* InstigatorActor)
{
	if (!bBossHPBarShown && Current < Max) // '첫 데미지' 시점만
	{
		bBossHPBarShown = true;
		if (HUDWidget)
		{
			HUDWidget->SetBossHPBarVisible(true);
		}
	}
}