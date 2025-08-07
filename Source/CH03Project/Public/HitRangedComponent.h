#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitRangedComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CH03PROJECT_API UHitRangedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitRangedComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComp;

public:
	UFUNCTION(BlueprintCallable)
	void FireTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitRanged")
	float TraceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitRanged")
	FVector StartOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitRanged")
	FName SocketName;

		
};
