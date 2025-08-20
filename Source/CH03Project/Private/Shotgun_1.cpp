#include "Shotgun_1.h"
#include "MyCharacter.h"
#include "DamageComponent.h"
#include "MyPlayerController.h"

AShotgun_1::AShotgun_1()
{
	PrimaryActorTick.bCanEverTick = false;

	PelletCount = 10;
	Spread = 20.0f;
}

void AShotgun_1::BeginPlay()
{
	Super::BeginPlay();

	WeaponName = "Shotgun_1";
	Damage = 15;
	AttackSpeed = 1.0f;
	MaxAmmo = 5;
	CurrentAmmo = MaxAmmo;
	ShootingRange = 10000.0f;
	ReloadingTime = 3.2f;
	RecoilPitchScale = 20.0f;
	RecoilYawScale = 5.0f;

	StaticMeshComp->AttachToComponent(
		SkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("magazine")
	);

	OnChangeCurrentAmmo.Broadcast(CurrentAmmo);
}

void AShotgun_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotgun_1::HitScan(AMyCharacter* WeaponOwner)
{
	if (!IsValid(WeaponOwner)) return;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(WeaponOwner);

	FVector Location;
	FRotator BaseRotation;
	WeaponOwner->GetMyPlayerController()->GetPlayerViewPoint(Location, BaseRotation);

	for (int32 i = 0; i < PelletCount; i++)
	{
		float RandYaw = FMath::FRandRange(-Spread, Spread);
		float RandPitch = FMath::FRandRange(-Spread, Spread);
		FRotator ShotRotation = BaseRotation + FRotator(RandPitch, RandYaw, 0.0f);
		FVector End = Location + (ShotRotation.Vector() * ShootingRange);

		TArray<FHitResult> HitResults;

		bool bDidHit = GetWorld()->LineTraceMultiByChannel(
			HitResults,
			Location,
			End,
			ECC_GameTraceChannel4,
			Params
		);

		if (bDidHit)
		{
			AActor* FirstHitActor = nullptr;
			bool bHeadshotFound = false;

			for (int32 j = 0; j < HitResults.Num(); j++)
			{
				const FHitResult& Hit = HitResults[j];

				if (j == 0)
				{
					if (Hit.GetActor() && Hit.GetActor()->ActorHasTag("Enemy"))
					{
						FirstHitActor = Hit.GetActor();
					}
					else
					{
						break;
					}
				}
				else
				{
					if (Hit.GetActor() != FirstHitActor)
					{
						break;
					}
				}

				if (Hit.GetComponent() && Hit.GetComponent()->ComponentHasTag("Head"))
				{
					if (UDamageComponent* OwnerDamageComp = WeaponOwner->DamageComp)
					{
						OwnerDamageComp->TransDamageCritical(FirstHitActor);
						bHeadshotFound = true; 
					}
					break;
				}
			}
			if (!bHeadshotFound && FirstHitActor)
			{
				if (UDamageComponent* OwnerDamageComp = WeaponOwner->DamageComp)
				{
					OwnerDamageComp->TransDamage(FirstHitActor);
				}
			}

			DrawDebugLine(
				GetWorld(), 
				Location + (ShotRotation.Vector() * 10.0f), 
				End,
				FColor::Orange, 
				false, 
				0.5f,
				1,
				0.2f);
		}
		else
		{
			DrawDebugLine(
				GetWorld(),
				Location + (ShotRotation.Vector() * 10.0f),
				End,
				FColor::Orange,
				false,
				0.5f,
				1,
				0.2f);
		}
	}
}
