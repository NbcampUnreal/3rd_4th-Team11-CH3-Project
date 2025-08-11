#include "HitRangedComponent.h"
#include "GameFramework/Actor.h"
#include "DamageComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UHitRangedComponent::UHitRangedComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHitRangedComponent::BeginPlay()
{
	Super::BeginPlay();


	AActor* Owner = GetOwner();
	if (Owner)
	{
		SkeletalMeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
	}
}


void UHitRangedComponent::FireTrace()
{
	if (!GetWorld())
	{
		return;
	}

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerChar)
	{
		CachedPlayerLocation = PlayerChar->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *CachedPlayerLocation.ToString());
	}
	else
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_FireTrace,
		this,
		&UHitRangedComponent::StartAttack,
		AttackDelayTime,
		false
	);
}

void UHitRangedComponent::StartAttack()
{
	if (!SkeletalMeshComp || !GetWorld())
	{
		return;
	}

	const FVector StartLocation = SkeletalMeshComp->GetSocketLocation(SocketName);
	const FVector EndLocation = CachedPlayerLocation;

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(FireTrace), false, GetOwner());
	TraceParams.bTraceComplex = false;

	FHitResult HitResult(ForceInit);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel1,
		TraceParams
	);

#if WITH_EDITOR

	DrawDebugLine(
		GetWorld(),
		StartLocation,
		EndLocation,
		bHit ? FColor::Red : FColor::Green,
		false,
		2.0f
	);
#endif

	
	if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Player"));

		if (UDamageComponent* DamageComp = GetOwner()->FindComponentByClass<UDamageComponent>())
		{
			DamageComp->TransDamage(HitResult.GetActor());
		}
	}
}