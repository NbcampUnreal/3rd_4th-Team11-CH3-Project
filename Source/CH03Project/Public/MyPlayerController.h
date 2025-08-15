#pragma once

#include "CoreMinimal.h"
#include "EndMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class UBaseStatComponent;
class UBaseItem;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InvAction;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UGunAccessory> GunAccessoryWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UGunAccessory* GunAccessory;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UEndMenuWidget* EndMenuWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UEndMenuWidget> EndMenuWidgetClass;

	UFUNCTION()
	void HandleAddItemChanged(FName ItemID, int32 Quantity);
	UFUNCTION() 
	void HandleRemoveItemChanged(FName ItemID, int32 Quantity);
	UFUNCTION()
	void HandleAddAccessoryChanged(FName ItemID);

	//---------------------CoolTime------------
	UFUNCTION()
	void HandleItemCoolTime(float Remaining, FName ItemId);
	void BindCoolTimeToItem(class UBaseItem* Item);
	void BindCoolTimeInInventory(class UInventoryComponent* Inv);

public:
	AMyPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPauseMenu();
	UFUNCTION()
	void OnInvMenu();
	
	void BindDeligateToSpawnedWeapon(AActor* SpawnedWeapon);

protected:
	bool bBossHPBarShown = false;

	UFUNCTION()
	void HandleBossHpChanged(int32 Current, int32 Max, AActor* InstigatorActor);

	virtual void SetupInputComponent() override;
};
