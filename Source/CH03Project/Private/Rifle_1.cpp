#include "Rifle_1.h"

ARifle_1::ARifle_1()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 10;
	AttackSpeed = 0.2f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}

void ARifle_1::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComp->AttachToComponent(
		SkeletalMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("magazine")
	);
}

void ARifle_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

