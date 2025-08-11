#include "BaseStatComponent.h"
#include "MenuWidget.h"
#include "HUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "GameStatePlay.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BaseEnemy.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"
#include "GameModePlay.h"


UBaseStatComponent::UBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ImmuneToDamageTime = 0.1f;
	MaxHp = 100;
	Hp = MaxHp;
	bIsDead = false;
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
	if (bIsImmuneToDamage)
	{
		return;
	}

	if (Point < 0)
	{
		ImmuneToDamageSet();
		Point = FMath::Min(Point + Armor, 0);

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AMyPlayerController* MyController = Cast<AMyPlayerController>(PC);

		ACharacter* MyPlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		AActor* DamagedActor = GetOwner();
		if (!bIsDead && DamagedActor != MyPlayerCharacter)
		{
			FVector DamageLocation = GetOwner()->GetActorLocation() + FVector(1, 1, 100.0f);
			if (MyController && MyController->HUDWidget)
			{
				MyController->HUDWidget->ShowDamageText(-Point, DamageLocation);
				MyController->HUDWidget->ShowHitMarker();
			}
		}
	}


	Hp += Point;
	Hp = FMath::Clamp(Hp, 0, MaxHp);

	//캐릭터와 보스	
	OnHpChangedEvent.Broadcast(Hp, MaxHp, GetOwner());

	const float HPRatio = (float)Hp / FMath::Max(1, MaxHp);
	if (!bTriggeredStun70 && HPRatio <= 0.70f)
	{
		bTriggeredStun70 = true;
		RaiseStunOnAI();
	} else if (!bTriggeredStun38 && HPRatio <= 0.38f)
	{
		bTriggeredStun38 = true;
		RaiseStunOnAI();
	}

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
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	if (AAIController* AICon = Cast<AAIController>(OwnerCharacter->GetController()))
	{
		if (AEnemyAIController* EnemyAICon = Cast<AEnemyAIController>(AICon))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy is dead!"));
			}
			EnemyAICon->SetStateAsDead();
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
				{
					if (MyPC->HUDWidget)
					{
						MyPC->HUDWidget->ShowKillMarker();
						
					}
				}
			}
		}
	}
	OnDeathEvent.Broadcast(OwnerCharacter);
	AGameModePlay* GameModePlay = Cast<AGameModePlay>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModePlay)
	{
		GameModePlay->AddScore(KillScore);
		GameModePlay->AddKillCount(1);
		//킬로그 전송필요
		//처치UI필요
	}


	AMyPlayerController* PC = Cast<AMyPlayerController>(OwnerCharacter->GetController());
	if (PC && PC->MainMenuWidgetClass)
	{
		UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(PC, PC->MainMenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->SetMenuState(true, 0);

			int32 FinalScore = 0;
			if (AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld())))
			{
				FinalScore = GameStatePlay->Score;
			}

			MenuWidget->SetScoreNumText(true, FinalScore);
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;
		}
	}

	OnDeathEvent.Clear();
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

void UBaseStatComponent::RaiseStunOnAI()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;
	if (!OwnerActor->HasAuthority()) return;
	APawn* Pawn = Cast<APawn>(OwnerActor);
	if (!Pawn) return;
	AAIController* AIC = Cast<AAIController>(Pawn->GetController());
	if (!AIC) return;
	UBlackboardComponent* BB = AIC->GetBlackboardComponent();
	if (!BB) return;

	BB->SetValueAsBool(ShouldStunBBKeyName, true);
}

