#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "BaseActor.generated.h"

class UBaseStatComponent;
class UDamageComponent;

UCLASS()
class CH03PROJECT_API ABaseActor : public ACharacter
{
	GENERATED_BODY()
	
public:
	ABaseActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBaseStatComponent* BaseStatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDamageComponent* DamageComponent;*/
};
