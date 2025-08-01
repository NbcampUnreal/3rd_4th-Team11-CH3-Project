// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPointSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "BaseEnemy.h"
#include "NavigationSystem.h"

// Sets default values
AWayPointSpawner::AWayPointSpawner()
    : SampRadius(1000.0f),
    WallCheckRadius(50.0f),
    MaxAttemptsPerEnemy(50),
    NumWayPoints(5)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

void AWayPointSpawner::GenerateWaypointVectorsForActor(ABaseEnemy* Enemy, TArray<FVector>& OutVectors)
{
    if (!Enemy) return;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return;

    FVector Origin = Enemy->GetActorLocation();
    int32 Count = 0;
    int32 Attempts = 0;

    while (Count < NumWayPoints && Attempts < MaxAttemptsPerEnemy)
    {
        Attempts++;

        FNavLocation NavLoc;
        if (NavSys->GetRandomReachablePointInRadius(Origin, SampRadius, NavLoc))
        {
            if (FVector::DistSquared(NavLoc.Location, Origin) < FMath::Square(200.f))
                continue;

            FVector Offset(80.f, 0.f, 0.f);
            FVector Start = NavLoc.Location - Offset + FVector(0.f, 0.f, 150.f);
            FVector End = NavLoc.Location + Offset + FVector(0.f, 0.f, 150.f);

            FHitResult Hit;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);
            Params.AddIgnoredActor(Enemy);

            bool bHit = GetWorld()->SweepSingleByChannel(
                Hit,
                Start,
                End,
                FQuat::Identity,
                ECC_Visibility,
                FCollisionShape::MakeSphere(WallCheckRadius),
                Params
            );

            if (bHit)
                continue;

            OutVectors.Add(NavLoc.Location);
            DrawDebugSphere(GetWorld(), NavLoc.Location, 50.f, 12, FColor::Cyan, true, 10.f);
            Count++;
        }
    }
}
