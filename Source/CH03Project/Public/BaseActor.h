#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStatComponent.h"
#include "BaseActor.generated.h"

UCLASS()
class CH03PROJECT_API ABaseActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBaseStatComponent* BaseStatComponent;

};
