#include "Rifle_1.h"

ARifle_1::ARifle_1()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARifle_1::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponName = "Rifle_1";
	Damage = 10;
	AttackSpeed = 0.15f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	ShootingRange = 10000.0f;
	ReloadingTime = 3.2f;
	RecoilPitchScale = 2.0f;
	RecoilYawScale = 0.4f;

	StaticMeshComp->AttachToComponent(
		SkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("magazine")
	);

	OnChangeCurrentAmmo.Broadcast(CurrentAmmo);
}

void ARifle_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

