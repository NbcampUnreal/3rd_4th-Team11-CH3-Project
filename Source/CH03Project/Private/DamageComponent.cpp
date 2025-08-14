#include "DamageComponent.h"
#include "BaseStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.h"
#include "HUDWidget.h"
#include "BaseWeaponInterface.h"
#include "Perception/AISense_Damage.h"

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageComponent::SetAttackDamage(IBaseWeaponInterface* EquippedWeapon)
{
	if (EquippedWeapon)
	{
		AttackDamage = EquippedWeapon->GetDamage();
	}
}

void UDamageComponent::SetAttackDamage(int32 NewAttackDamage)
{
	AttackDamage = NewAttackDamage;
}

int32 UDamageComponent::GetAttackDamage() const
{
	return AttackDamage;
}

void UDamageComponent::TransDamage(AActor* TargetActor)
{
	if (TargetActor)
	{
		UBaseStatComponent* TargetStatComponent = TargetActor->FindComponentByClass<UBaseStatComponent>();
		if (TargetStatComponent)
		{
			TargetStatComponent->AddHp(-AttackDamage);

			APawn* InstigatorPawn = Cast<APawn>(GetOwner());
			if (InstigatorPawn && TargetActor->HasAuthority())
			{
				UAISense_Damage::ReportDamageEvent(
					GetWorld(),
					TargetActor,           
					InstigatorPawn,          
					static_cast<float>(AttackDamage),
					TargetActor->GetActorLocation(),
					InstigatorPawn->GetActorLocation()
				);
			}
		}
	}
}

bool UDamageComponent::StoreAttackToken(int32 Amount)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		return true;
	}
	return false;
}

void UDamageComponent::ReturnAttackToken(int32 Amount)
{
	AttackTokenCount += Amount;
}
