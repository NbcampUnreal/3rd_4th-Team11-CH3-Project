#include "HitRangedComponent.h"
#include "GameFramework/Actor.h"
#include "DamageComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"


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
	if (!SkeletalMeshComp)
	{
		return;
	}
	FVector SocketLocation = SkeletalMeshComp->GetSocketLocation(SocketName);
	FRotator SocketRotation = SkeletalMeshComp->GetSocketRotation(SocketName);
	FVector SocketForward = SocketRotation.Vector();

	FVector StartLocation = SocketLocation + (SocketForward * StartOffset.X);
	FVector EndLocation = StartLocation + (SocketForward * TraceDistance);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.bTraceComplex = false;


	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, TraceParams);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHit ? FColor::Red : FColor::Green, false, 2.0f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
		if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Player"));
			UDamageComponent* DamageComp = HitResult.GetActor()->FindComponentByClass<UDamageComponent>();
			if (DamageComp)
			{
				DamageComp->TransDamage(HitResult.GetActor());
			}
		}
	}

}

