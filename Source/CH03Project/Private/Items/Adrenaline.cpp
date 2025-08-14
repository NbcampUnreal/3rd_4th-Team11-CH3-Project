#include "Items/Adrenaline.h"
#include "BaseStatComponent.h"
#include "DamageComponent.h"
#include "MyCharacter.h"

UAdrenaline::UAdrenaline()
{
	NewDamageStat = 15;
	NewArmorStat = 3;
	Duration = 5.0f;
}

bool UAdrenaline::Use_Implementation(AActor* User)
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
		// 데미지 컴포넌트 가져오기
		UDamageComponent* DamageComponent = Player->FindComponentByClass<UDamageComponent>();
		// 스탯 컴포넌트 가져오기
		UBaseStatComponent* StatComponent = Player->FindComponentByClass<UBaseStatComponent>();

		if(StatComponent && DamageComponent)
		{
			OldDamageStat = DamageComponent->GetAttackDamage();
			OldArmorStat = StatComponent->GetArmor();
			DamageComponent->SetAttackDamage(NewDamageStat);
			StatComponent->SetArmor(NewArmorStat);
			UE_LOG(LogTemp, Warning, TEXT("%d -> %d Damage Stat Increase!"), OldDamageStat, NewDamageStat);
			UE_LOG(LogTemp, Warning, TEXT("%d -> %d Armor Stat Increase!"),OldArmorStat, NewArmorStat);
		}

		// 타이머 델리게이트 생성해서 파라미터 전달
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("ResetAdrenalineEffect"), User);

		Player->GetWorld()->GetTimerManager().SetTimer(
			DurationTimerHandle,
			TimerDelegate,
			Duration,
			false
		);
	}
	return true;
}

// 아이템 효과를 지속시간 후에 해제
void UAdrenaline::ResetAdrenalineEffect(AActor* User)
{
	AMyCharacter* Player = Cast<AMyCharacter>(User);
	if(Player)
	{
		UDamageComponent* DamageComponent = Player->FindComponentByClass<UDamageComponent>();
		UBaseStatComponent* StatComponent = Player->FindComponentByClass<UBaseStatComponent>();
		if(StatComponent && DamageComponent)
		{
			DamageComponent->SetAttackDamage(OldDamageStat);
			StatComponent->SetArmor(OldArmorStat);
			UE_LOG(LogTemp, Warning, TEXT("Adrenaline effect reset. Damage stat restored to %d."), OldDamageStat);
			UE_LOG(LogTemp, Warning, TEXT("Adrenaline effect reset. Armor stat restored to %d."), OldArmorStat);
		}

	}
}
