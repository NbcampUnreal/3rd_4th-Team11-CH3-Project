#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UBaseStatComponent;

UCLASS()
class CH03PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	//-----------------Input---------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractionAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;
	//-----------------UI--------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UHUDWidget> HUDWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UHUDWidget* HUDWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UPauseWidget> PauseWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UPauseWidget* PauseWidget;

	UFUNCTION()
	void HandleAddItemChanged(FName ItemID, int32 Quantity);
	UFUNCTION() 
	void HandleRemoveItemChanged(FName ItemID, int32 Quantity);

	UFUNCTION()
	void HandleAddAccessoryChanged(FName ItemID);

public:
	AMyPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPauseMenu();

	void BindDeligateToSpawnedWeapon(AActor* SpawnedWeapon);

protected:
	bool bBossHPBarShown = false;

	UFUNCTION()
	void HandleBossHpChanged(int32 Current, int32 Max, AActor* InstigatorActor);
};
