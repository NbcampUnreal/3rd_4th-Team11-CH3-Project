#include "BaseStatComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BaseEnemy.h"
#include "EnemyAIController.h"


UBaseStatComponent::UBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ImmuneToDamageTime = 0.1f;
	Bullet = 30;
}


void UBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();
	OnHpChangedEvent.Broadcast(Hp, MaxHp, GetOwner());
}



//void UBaseStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

void UBaseStatComponent::AddHp(int Point)
{
	if (Point < 0)
	{
		ImmuneToDamageSet();
		Point = FMath::Min(Point + Armor, 0);
	}


	Hp += Point;
	Hp = FMath::Clamp(Hp, 0, MaxHp);

	//캐릭터와 보스	
	OnHpChangedEvent.Broadcast(Hp, MaxHp, GetOwner());

	if (Hp <= 0)
	{
		OnDeath();
	}
}

void UBaseStatComponent::AddMaxHp(int Point)
{
	MaxHp += Point;
	MaxHp = FMath::Max(0, MaxHp);
	Hp = FMath::Min(Hp, MaxHp);

	//현재 체력도 같은 포인트 늘려줄지 고민해봐야한다.
}

void UBaseStatComponent::AddArmor(int Point)
{
	Armor += Point;
	Armor = FMath::Max(0, Armor);	//방어가 0이하로 내려갈 수 있게?
}


void UBaseStatComponent::ImmuneToDamageSet()
{
	bIsImmuneToDamage = true;
	//타이머를 이용하여 time후에 ImmuneToDamageEnd를 호출
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBaseStatComponent::ImmuneToDamageEnd, ImmuneToDamageTime, false);
}

void UBaseStatComponent::ImmuneToDamageEnd()
{
	bIsImmuneToDamage = false;
}

void UBaseStatComponent::OnDeath()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	OnDeathEvent.Broadcast(OwnerCharacter);

	if (USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh())
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (AAIController* AICon = Cast<AAIController>(OwnerCharacter->GetController()))
	{
		if (UBrainComponent* BrainComp = AICon->GetBrainComponent())
		{
			BrainComp->StopLogic(TEXT(""));
		}

		if (AEnemyAIController* EnemyAICon = Cast<AEnemyAIController>(AICon))
		{
			EnemyAICon->SetStateAsDead();
		}
	}
}

int UBaseStatComponent::GetHp()
{
	return Hp;
}

int UBaseStatComponent::GetMaxHp()
{
	return MaxHp;
}

int UBaseStatComponent::GetArmor()
{
	return Armor;
}