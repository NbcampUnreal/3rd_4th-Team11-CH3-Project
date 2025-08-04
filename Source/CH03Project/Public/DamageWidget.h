#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"


UCLASS()
class CH03PROJECT_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Damage") 
	void SetDamageText(int32 DamageAmount);

	void SetScreenPosition(const FVector2D& ScreenPos);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
};
