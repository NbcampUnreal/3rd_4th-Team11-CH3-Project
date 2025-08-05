#include "HitMeleeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "DamageComponent.h"

UHitMeleeComponent::UHitMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HitMeleeCapsuleLeft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitMeleeSphereLeft"));
	HitMeleeCapsuleLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OffsetLocationLeft = FVector(10.0f, 0.0f, 25.0f);
	

	HitMeleeCapsuleRight = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitMeleeSphereRight"));
	HitMeleeCapsuleRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OffsetLocationRight = FVector(-10.0f, 0.0f, -25.0f);
	
}


// Called when the game starts
void UHitMeleeComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshComp)
		{
			HitMeleeCapsuleLeft->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketNameLeft);
			HitMeleeCapsuleLeft->SetRelativeLocation(OffsetLocationLeft);

			HitMeleeCapsuleRight->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketNameRight);
			HitMeleeCapsuleRight->SetRelativeLocation(OffsetLocationRight);
		}
	}
	 
	HitMeleeCapsuleLeft->OnComponentBeginOverlap.AddDynamic(this, &UHitMeleeComponent::OnComponentBeginOverlap);
	HitMeleeCapsuleRight->OnComponentBeginOverlap.AddDynamic(this, &UHitMeleeComponent::OnComponentBeginOverlap);
}


void UHitMeleeComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit detected with"));
	if (OtherActor && OtherActor != GetOwner() && OtherActor->ActorHasTag(TEXT("Player")))
	{	
		UDamageComponent* DamageComp = GetOwner()->FindComponentByClass<UDamageComponent>();

		if (DamageComp)
		{
			DamageComp->TransDamage(OtherActor);
		}
	}
}


void UHitMeleeComponent::SetCollisionEnabled(bool bEnabled)
{
	if (HitMeleeCapsuleLeft)
	{
		if (bEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("True"));
			HitMeleeCapsuleLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("False"));
			HitMeleeCapsuleLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
	}
	if (HitMeleeCapsuleRight)
	{
		if (bEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("True-L"));
			HitMeleeCapsuleRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("False-R"));
			HitMeleeCapsuleRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
}