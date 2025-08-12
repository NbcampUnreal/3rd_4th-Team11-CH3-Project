// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropItemTable.h"
#include "DropComponent.generated.h"

class UBaseStatComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UDropComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDropComponent();

    UPROPERTY(EditAnywhere, Category = "Drop")
    TSoftObjectPtr<UDropItemTable> DropTable;

    UPROPERTY(EditAnywhere, Category = "Drop")
    int32 SeedOffset = 0;

    UPROPERTY(EditAnywhere, Category = "Drop", meta = (ClampMin = "0.0"))
    float ScatterRadius = 150.f;

    UFUNCTION(BlueprintCallable, Category = "Drop")
    void DropAtLocation(const FVector& DeathLocation);

private:
    int32 MaxGroundProbes = 6;
    float ProbeUp = 80.f;
    float ProbeDown = 600.f;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    struct FCachedEntry
    {
        TSoftClassPtr<AActor> ItemClass;
        float CumulativeWeight = 0.f;
        int32 MinCount = 1;
        int32 MaxCount = 1;
    };

    TArray<FCachedEntry> Cached;
    float TotalWeight = 0.f;
    bool  bCacheBuilt = false;
    FRandomStream RNG;

    void BuildCacheIfNeeded();
    int32 RollDropCount() const;
    const FCachedEntry* PickOne();
    bool FindGround(const FVector& Around, FVector& Out) const;
    void SpawnItem(const FCachedEntry& E, const FVector& Center);

    void TryBindDeath();
    void UnbindDeath();

    bool bDropTriggered = false;

    TWeakObjectPtr<UBaseStatComponent> StatCompWeak;

    UFUNCTION() void OnOwnerDeath_FromStats(AActor* DeadActor);
    UFUNCTION() void OnOwnerDestroyed_Handler(AActor* DestroyedActor);
};
