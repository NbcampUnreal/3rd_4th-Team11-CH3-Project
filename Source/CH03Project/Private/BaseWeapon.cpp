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

void ABaseWeapon::Attack(AActor* TargetActor)
{

}

void ABaseWeapon::Equip(AActor* TargetActor)
{

}

void ABaseWeapon::Unequip(AActor* TargetActor)
{

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
	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

