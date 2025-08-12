// Fill out your copyright notice in the Description page of Project Settings.

#include "DropComponent.h"
#include "BaseStatComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"

UDropComponent::UDropComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UDropComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        if (Owner->HasAuthority())
        {
            const uint64 UID = static_cast<uint64>(Owner->GetUniqueID());
            RNG.Initialize(static_cast<int32>((UID ^ 0x9E3779B97F4A7C15ull) + SeedOffset));

            BuildCacheIfNeeded();
            TryBindDeath();
        }
    }
}

void UDropComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnbindDeath();
    Super::EndPlay(EndPlayReason);
}

void UDropComponent::BuildCacheIfNeeded()
{
    if (bCacheBuilt) return;

    Cached.Reset();
    TotalWeight = 0.f;

    if (UDropItemTable* Table = DropTable.LoadSynchronous())
    {
        Cached.Reserve(Table->Rows.Num());
        for (const FDropItemRow& R : Table->Rows)
        {
            if (R.Weight <= 0.f || !R.ItemClass.IsValid()) continue;

            TotalWeight += R.Weight;

            FCachedEntry E;
            E.ItemClass = R.ItemClass;
            E.CumulativeWeight = TotalWeight;
            E.MinCount = R.MinCount;
            E.MaxCount = R.MaxCount;
            Cached.Add(E);
        }
    }

    bCacheBuilt = true;
}

int32 UDropComponent::RollDropCount() const
{
    const UDropItemTable* Table = DropTable.Get();
    if (!Table) return 0;

    const int32 Min = FMath::Max(0, Table->MinItemType);
    const int32 Max = FMath::Max(Min, Table->MaxItemType);
    return RNG.RandRange(Min, Max);
}

const UDropComponent::FCachedEntry* UDropComponent::PickOne()
{
    if (Cached.Num() == 0 || TotalWeight <= 0.f) return nullptr;

    const float R = RNG.FRand() * TotalWeight;
    for (const FCachedEntry& E : Cached)
    {
        if (R <= E.CumulativeWeight) return &E;
    }
    return &Cached.Last();
}
bool UDropComponent::FindGround(const FVector& Around, FVector& Out) const
{
    if (UWorld* World = GetWorld())
    {
        const float Angle = RNG.FRandRange(0.f, 360.f);
        const FVector2D Dir2D(FMath::Cos(FMath::DegreesToRadians(Angle)),FMath::Sin(FMath::DegreesToRadians(Angle)));
        const FVector Offset = FVector(Dir2D.X, Dir2D.Y, 0.f).GetSafeNormal() * RNG.FRandRange(0.f, ScatterRadius);

        const FVector Start = Around + Offset + FVector(0, 0, ProbeUp);
        const FVector End = Around + Offset - FVector(0, 0, ProbeDown);

        FHitResult Hit;
        FCollisionQueryParams Q(SCENE_QUERY_STAT(DropGround), false, GetOwner());
        FCollisionObjectQueryParams Obj;
        Obj.AddObjectTypesToQuery(ECC_WorldStatic);
        Obj.AddObjectTypesToQuery(ECC_WorldDynamic);

        if (World->LineTraceSingleByObjectType(Hit, Start, End, Obj, Q))
        {
            Out = Hit.ImpactPoint + FVector(0, 0, 2.f);
            return true;
        }
    }
    return false;
}

void UDropComponent::SpawnItem(const FCachedEntry& E, const FVector& Center)
{
    UClass* Cls = E.ItemClass.LoadSynchronous();
    if (!Cls) return;

    FVector SpawnLoc = Center;
    for (int32 Try = 0; Try < MaxGroundProbes; ++Try)
    {
        if (FindGround(Center, SpawnLoc)) break;
    }

    const FRotator Rot(0.f, RNG.FRandRange(0.f, 360.f), 0.f);

    FActorSpawnParameters SP;
    SP.Owner = GetOwner();
    SP.Instigator = Cast<APawn>(GetOwner());
    SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(Cls, SpawnLoc, Rot, SP);
}

void UDropComponent::DropAtLocation(const FVector& DeathLocation)
{
    if (!GetOwner() || !GetOwner()->HasAuthority()) return;

    BuildCacheIfNeeded();

    const int32 NumDrops = RollDropCount();
    for (int32 i = 0; i < NumDrops; ++i)
    {
        if (const FCachedEntry* Pick = PickOne())
        {
            SpawnItem(*Pick, DeathLocation);
        }
    }
}

void UDropComponent::TryBindDeath()
{
    AActor* Owner = GetOwner();
    if (!Owner || !Owner->HasAuthority()) return;

    if (UBaseStatComponent* Stat = Owner->FindComponentByClass<UBaseStatComponent>())
    {
        StatCompWeak = Stat;
        Stat->OnDeathEvent.RemoveDynamic(this, &UDropComponent::OnOwnerDeath_FromStats);
        Stat->OnDeathEvent.AddDynamic(this, &UDropComponent::OnOwnerDeath_FromStats);
    }

    Owner->OnDestroyed.RemoveDynamic(this, &UDropComponent::OnOwnerDestroyed_Handler);
    Owner->OnDestroyed.AddDynamic(this, &UDropComponent::OnOwnerDestroyed_Handler);
}

void UDropComponent::UnbindDeath()
{
    if (AActor* Owner = GetOwner())
    {
        Owner->OnDestroyed.RemoveDynamic(this, &UDropComponent::OnOwnerDestroyed_Handler);
    }
    if (UBaseStatComponent* Stat = StatCompWeak.Get())
    {
        Stat->OnDeathEvent.RemoveDynamic(this, &UDropComponent::OnOwnerDeath_FromStats);
    }
}

void UDropComponent::OnOwnerDeath_FromStats(AActor* DeadActor)
{
    AActor* Owner = GetOwner();
    if (!Owner || !Owner->HasAuthority()) return;
    if (DeadActor != Owner) return;
    if (bDropTriggered) return;

    bDropTriggered = true;
    DropAtLocation(Owner->GetActorLocation());
}

void UDropComponent::OnOwnerDestroyed_Handler(AActor* DestroyedActor)
{
    AActor* Owner = GetOwner();
    if (!Owner || !Owner->HasAuthority()) return;
    if (bDropTriggered) return;

    bDropTriggered = true;
    DropAtLocation(DestroyedActor ? DestroyedActor->GetActorLocation() : Owner->GetActorLocation());
}
