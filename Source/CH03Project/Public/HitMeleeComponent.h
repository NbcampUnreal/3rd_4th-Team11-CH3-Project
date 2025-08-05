#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitMeleeComponent.generated.h"

class UCapsuleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UHitMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitMeleeComponent();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	UCapsuleComponent* HitMeleeCapsuleLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	FName SocketNameLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	FVector OffsetLocationLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	UCapsuleComponent* HitMeleeCapsuleRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	FName SocketNameRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMelee")
	FVector OffsetLocationRight;



	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetCollisionEnabled(bool bEnabled);
	
};
