#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MyCharacter.h"
#include "HUDWidget.h"
#include "BaseStatComponent.h"

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

    if (HUDWidgetClass)
    {
        UHUDWidget* MyHUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
        if (MyHUDWidget)
		{
            MyHUDWidget->AddToViewport();
            AMyCharacter* MyPlayerCharacter = Cast<AMyCharacter>(GetPawn());
            if (MyPlayerCharacter)
            {
                UBaseStatComponent* StatComponent = MyPlayerCharacter->FindComponentByClass<UBaseStatComponent>();
                if (StatComponent)
                {
                    StatComponent->OnHpChangedEvent.AddDynamic(MyHUDWidget, &UHUDWidget::UpdateHealth);

                    MyHUDWidget->UpdateHealth(StatComponent->GetHp(), StatComponent->GetMaxHp(), GetPawn());
                }
            }
        }
    }
}