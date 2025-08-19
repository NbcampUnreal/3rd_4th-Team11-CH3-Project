#include "Items/Adrenaline.h"
#include "BaseStatComponent.h"
#include "DamageComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UAdrenaline::UAdrenaline()
{
	IncreaseDamageStat = 5;
	IncreaseArmorStat = 3;
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
		//UE_LOG(LogTemp, Warning, TEXT("No User!"));
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
			DamageComponent->SetItemPlusDamage(IncreaseDamageStat);
			StatComponent->SetArmor(IncreaseArmorStat);
			//UE_LOG(LogTemp, Warning, TEXT("%d Damage Stat Increase!"), IncreaseDamageStat);
			//UE_LOG(LogTemp, Warning, TEXT("%d Armor Stat Increase!"), IncreaseArmorStat);
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

	// 데이터테이블 핸들이 유효한지
	if(!ItemDataHandle.DataTable || !ItemDataHandle.RowName.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("CatStatue: No valid ItemDataHandle"));
		return false;
	}

	// 데이터 테이블에서 Row 가져오기
	FItemDataRow* ItemRow =
		ItemDataHandle.DataTable->FindRow<FItemDataRow>(
			ItemDataHandle.RowName,
			TEXT("CatStatue Use_Implementation")
		);

	if(!ItemRow)
	{
		return false;
	}

	// 사운드 경로 설정 확인
	if(!ItemRow->UseSound.ToSoftObjectPath().IsValid())
	{
		return false;
	}

	// 메모리 상에 로드가 안 되어있으면 LoadSynchronous
	USoundBase* Sound = nullptr;
	if(ItemRow->UseSound.IsValid())
	{
		// 이미 메모리에 로드됨
		Sound = ItemRow->UseSound.Get();
	}
	else
	{
		// 로드되지 않았으면 동기 로드
		Sound = ItemRow->UseSound.LoadSynchronous();
	}

	// 재생
	if(Sound && User)
	{
		UGameplayStatics::PlaySoundAtLocation(User, Sound, User->GetActorLocation());
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
			DamageComponent->SetItemPlusDamage(0);
			StatComponent->SetArmor(-IncreaseArmorStat);
			//UE_LOG(LogTemp, Warning, TEXT("Adrenaline effect reset."));
		}

	}
}
