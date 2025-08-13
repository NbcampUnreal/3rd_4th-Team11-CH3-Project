#include "Items/HealthPotion.h"
#include "GameFramework/Character.h"
#include "BaseStatComponent.h"
#include "MyCharacter.h"

UHealthPotion::UHealthPotion()
{
	HealAmount = 30.0f; // 초기 회복량 30
}
bool UHealthPotion::Use_Implementation(AActor* User)
{
	if(!Super::Use_Implementation(User))
	{
		return false;
	}

	if(!User)
	{
		UE_LOG(LogTemp, Warning, TEXT("No User!"));
		return false;
	}

	AMyCharacter* Player = Cast<AMyCharacter>(User);
	if(Player)
	{
		// 플레이어에 스탯 컴포넌트를 찾음
		UBaseStatComponent* StatComponent = Player->FindComponentByClass<UBaseStatComponent>();
		if(StatComponent)
		{
			StatComponent->HealHP(HealAmount);
			UE_LOG(LogTemp, Warning, TEXT("HealHP!"));
		}
	}
	return true;
}

