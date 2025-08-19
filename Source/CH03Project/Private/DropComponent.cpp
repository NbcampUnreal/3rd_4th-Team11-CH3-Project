// Fill out your copyright notice in the Description page of Project Settings.

#include "DropComponent.h"
#include "BaseStatComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
            TryBindDeath();
        }
    }
}

void UDropComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnbindDeath();
    Super::EndPlay(EndPlayReason);
}

int32 UDropComponent::PickIndexByWeight(const TArray<FDropItemRow>& Rows) const
{
    TArray<int32> ValidIdx;
    ValidIdx.Reserve(Rows.Num());

    float TotalW = 0.f;
    for (int32 i = 0; i < Rows.Num(); ++i)
    {
        if (!Rows[i].ItemClass) continue;
        ValidIdx.Add(i);
        TotalW += FMath::Max(0.f, Rows[i].Weight);
    }

    if (ValidIdx.Num() == 0) return INDEX_NONE;

    if (TotalW <= KINDA_SMALL_NUMBER)
    {
        return ValidIdx[RNG.RandRange(0, ValidIdx.Num() - 1)];
    }

    const float R = RNG.FRand() * TotalW;
    float Acc = 0.f;
    for (int32 i : ValidIdx)
    {
        Acc += FMath::Max(0.f, Rows[i].Weight);
        if (R <= Acc) return i;
    }
    return ValidIdx.Last();
}

int32 UDropComponent::RollDropCount() const
{
    const UDropItemTable* Table = DropTable.Get();
    if (!Table) return 0;

    const int32 Min = FMath::Max(0, Table->MinItemType);
    const int32 Max = FMath::Max(Min, Table->MaxItemType);
    if (RNG.FRand() < 0.5f)
    {
        return 0;
    }

    return (Max >= Min) ? RNG.RandRange(Min, Max) : 0;
}

void UDropComponent::SpawnItemClass(TSubclassOf<AActor> Cls, const FVector& Center)
{
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

bool UDropComponent::FindGround(const FVector& Around, FVector& Out) const
{
    if (UWorld* World = GetWorld())
    {
        const float Angle = RNG.FRandRange(0.f, 360.f);
        const FVector2D Dir2D(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)));
        const FVector Offset = FVector(Dir2D.X, Dir2D.Y, 0.f).GetSafeNormal() * RNG.FRandRange(0.f, ScatterRadius);

        const FVector Start = Around + Offset + FVector(0, 0, ProbeUp);
        const FVector End = Around + Offset - FVector(0, 0, ProbeDown);

        FCollisionQueryParams Params(SCENE_QUERY_STAT(DropGround), false, GetOwner());
        Params.bReturnPhysicalMaterial = false;

        // "Item" 태그를 가진 액터는 무시
        TArray<AActor*> ItemsToIgnore;
        UGameplayStatics::GetAllActorsWithTag(World, FName("Item"), ItemsToIgnore);
        Params.AddIgnoredActors(ItemsToIgnore);

        // 오브젝트 타입: 바닥만 체크
        FCollisionObjectQueryParams ObjParams;
        ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);   // 지형, 건물
        ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);  // 움직이는 플랫폼 같은 거

        FHitResult Hit;
        if (World->LineTraceSingleByObjectType(Hit, Start, End, ObjParams, Params))
        {
            // 바닥 충돌 지점 그대로 사용
            Out = Hit.Location;
            return true;
        }
    }
    return false;
}

void UDropComponent::SpawnItem(const FCachedEntry& E, const FVector& Center)
{
    UClass* Cls = E.ItemClass;
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

    const UDropItemTable* Table = DropTable.LoadSynchronous();
    if (!Table || Table->Rows.Num() == 0) return;

    const int32 TypeCount = RollDropCount();
    for (int32 t = 0; t < TypeCount; ++t)
    {
        const int32 RowIdx = PickIndexByWeight(Table->Rows);
        if (RowIdx == INDEX_NONE) continue;

        const FDropItemRow& Row = Table->Rows[RowIdx];
        UClass* Cls = Row.ItemClass;
        if (!Cls) continue;

        const int32 MinC = FMath::Max(1, Row.MinCount);
        const int32 MaxC = FMath::Max(MinC, Row.MaxCount);
        const int32 CountToSpawn = RNG.RandRange(MinC, MaxC);

        for (int32 i = 0; i < CountToSpawn; ++i)
        {
            SpawnItemClass(Cls, DeathLocation);
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