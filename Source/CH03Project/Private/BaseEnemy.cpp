// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WayPointSpawner.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolIndex = 0;
	Direction = 1;
	bIsWeildingWeapon = false;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPointSpawner::StaticClass(), FoundSpawners);

	for (AActor* Spawner : FoundSpawners)
	{
		if (AWayPointSpawner* WaySpawner = Cast<AWayPointSpawner>(Spawner))
		{
			WaySpawner->GenerateWaypointVectorsForActor(this, TargetPoints);
			break;
		}
	}
}

void ABaseEnemy::IncrementPatrolRoute()
{
	PatrolIndex += Direction;

	const int32 LastIndex = TargetPoints.Num() - 1;
	if (PatrolIndex >= LastIndex)
	{
		PatrolIndex = LastIndex;
		Direction = -1;
	}
	else if (PatrolIndex <= 0)
	{
		PatrolIndex = 0;
		Direction = 1;
	}
}

void ABaseEnemy::ShuffleWayPoints()
{
	if (TargetPoints.Num() > 1)
	{
		for (int32 i = 0; i < TargetPoints.Num() - 1; ++i)
		{
			int32 SwapIndex = FMath::RandRange(i + 1, TargetPoints.Num() - 1);
			TargetPoints.Swap(i, SwapIndex);
		}
	}
}

TArray<FVector> ABaseEnemy::GetPatrolWaypoints_Implementation()
{
	return TargetPoints;
}

void ABaseEnemy::SetMovementSpeed_Implementation(ESpeedState SpeedState)
{
	if (!CharacterMovementComponent) return;

	switch (SpeedState)
	{
	case ESpeedState::Idle:
		CharacterMovementComponent->MaxWalkSpeed = 0.0f;
		break;

	case ESpeedState::Walking:
		CharacterMovementComponent->MaxWalkSpeed = 100.0f;
		break;

	case ESpeedState::Jogging:
		CharacterMovementComponent->MaxWalkSpeed = 300.0f;
		break;

	case ESpeedState::Sprinting:
		CharacterMovementComponent->MaxWalkSpeed = 500.0f;
		break;

	default:
		CharacterMovementComponent->MaxWalkSpeed = 300.0f;
		break;
	}
}

void ABaseEnemy::GetIdealRadius_Implementation(float& OutAttackRadius, float& OutDefendRadius)
{
	OutAttackRadius = 150.f;
	OutDefendRadius = 350.f;
}

void ABaseEnemy::EquipWeapon_Implementation()
{
	bIsWeildingWeapon = true;
}

void ABaseEnemy::UnequipWeapon_Implementation()
{
	bIsWeildingWeapon = false;
}

void ABaseEnemy::Attack_Implementation()
{
}
