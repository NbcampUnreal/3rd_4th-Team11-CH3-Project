#include "BaseWeapon.h"
#include "Engine/Engine.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(SceneComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	Damage = 0;
	AttackSpeed = 0.0f;
}

USkeletalMeshComponent* ABaseWeapon::GetSkeletalMeshComponent()
{
	return SkeletalMeshComp;
}

UStaticMeshComponent* ABaseWeapon::GetStaticMeshComponent()
{
	return StaticMeshComp;
}

FName ABaseWeapon::GetWeaponType()
{
	return WeaponType;
}

FName ABaseWeapon::GetWeaponName()
{
	return WeaponName;
}

int32 ABaseWeapon::GetDamage()
{
	return Damage;
}

float ABaseWeapon::GetAttackSpeed()
{
	return AttackSpeed;
}

void ABaseWeapon::ChangeDamage(int32 NewDamage)
{
	Damage = NewDamage;
}

void ABaseWeapon::ChangeAttackSpeed(float NewAttackSpeed)
{
	AttackSpeed = NewAttackSpeed;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

