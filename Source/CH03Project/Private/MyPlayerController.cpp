#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
{
	IMC = nullptr;
	MoveAction = nullptr;
	JumpAction = nullptr;
	CrouchAction = nullptr;
	LookAction = nullptr;
	ShootAction = nullptr;
	AimAction = nullptr;
	ReloadAction = nullptr;
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
}