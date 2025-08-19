#include "BaseWeapon.h"

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

void ABaseWeapon::Attack()
{

}

void ABaseWeapon::Equip()
{

}

void ABaseWeapon::Unequip()
{
	
}

FName ABaseWeapon::GetWeaponType() const
{
	return WeaponType;
}

FName ABaseWeapon::GetWeaponName() const
{
	return WeaponName;
}

int32 ABaseWeapon::GetDamage() const
{
	return Damage;
}

float ABaseWeapon::GetAttackSpeed() const
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

