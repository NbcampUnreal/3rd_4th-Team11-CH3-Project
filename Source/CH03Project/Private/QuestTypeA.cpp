#include "QuestTypeA.h"
#include "GameModePlay.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DataTable.h"

AQuestTypeA::AQuestTypeA()
{	
	PrimaryActorTick.bCanEverTick = false;
	DoorCount = 0;
}

void AQuestTypeA::BeginPlay()
{
	Super::BeginPlay();
	GameModePlays = Cast<AGameModePlay>(UGameplayStatics::GetGameMode(GetWorld()));
	
}

// 시작 콜리전 동작시
void AQuestTypeA::StartQuest()
{
	// 퀘스트 시작 로직
	UE_LOG(LogTemp, Warning, TEXT("퀘스트가 시작되었습니다."));
	DoorCount++;
	//텍스트0 인스턴스로 전달 - 예상 텍스트(문을 열고 나아가라? -> 열쇠를 찾아라)
	GameModePlays->SetMissionText(QuestTexts[0].ToString(), 1);
}


void AQuestTypeA::CheckQuestDoor()
{
	if (DoorCount == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("1구역 문 열림"));
		DoorCount++;
		GameModePlays->SetMissionText(QuestTexts[1].ToString(), 1);
		//문 열림 처리 필요
		//공간을 지나가면 닫힘 필요
	}
	if (DoorCount == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("2구역 문 열림 카운트 시작"));
		DoorCount++;
		GameModePlays->SetMissionText(QuestTexts[3].ToString() + FString::SanitizeFloat(DoorOpenCount), 0);
		//타이머설치
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AQuestTypeA::DoorCountCheck, 0.1f, true);
		//문 열림 처리 필요
		//공간을 지나가면 닫힘 필요
	}
}

//다른 방식을 써야할지도 모름 UI에서 처리하는 방식에 따라서..
void AQuestTypeA::DoorCountCheck()
{
	//문열림 카운트 체크
	if (DoorOpenCount > 0)
	{
		DoorOpenCount -= 1;
		
		GameModePlays->SetMissionText(QuestTexts[3].ToString() + FString::SanitizeFloat(DoorOpenCount), 0);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		DoorOpenCount = 0;
		GameModePlays->SetMissionText(QuestTexts[4].ToString(), 1);
	}
}



//구역2에서 키를 얻으면 아래 함수를 호출
void AQuestTypeA::SpawnEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("몬스터가 소환이 시작되었습니다."));

	GameModePlays->SetMissionText(QuestTexts[2].ToString(), 1);

	// 스폰 위치에서 몬스터 소환
	int32 MaxSpawnCount = FMath::Min(SpawnLocation.Num(), SpawnDataArray.Num());

	for (int32 i = 0; i < MaxSpawnCount; ++i)
	{
		// 해당 인덱스의 SpawnLocation 박스가 유효한지 확인
		UBoxComponent* CurrentSpawnBox = SpawnLocation[i];
		if (!CurrentSpawnBox)
		{	
			continue;
		}

		// 해당 인덱스의 데이터 테이블이 유효한지 확인
		UDataTable* CurrentDataTable = SpawnDataArray[i];
		if (!CurrentDataTable)
		{
			continue;
		}

		// 데이터 테이블에서 모든 행을 가져옵니다.
		TArray<FEnemySpawnRow*> AllSpawnRowsInTable;
		CurrentDataTable->GetAllRows<FEnemySpawnRow>(TEXT("SpawnEnemy"), AllSpawnRowsInTable);

		if (AllSpawnRowsInTable.Num() == 0)
		{	
			continue;
		}

		// 데이터 테이블의 각 행(FEnemySpawnRow)을 순회하며 몬스터 스폰
		for (const FEnemySpawnRow* CurrentEnemyRow : AllSpawnRowsInTable)
		{
			if (!CurrentEnemyRow)
			{
				continue;
			}

			// 스폰할 몬스터 클래스가 유효한지 확인
			if (!CurrentEnemyRow->EnemyClass)
			{
				continue;
			}

			// 해당 몬스터 클래스를 SpawnCount만큼 스폰
			for (int32 j = 0; j < CurrentEnemyRow->SpawnCount; ++j)
			{
				FVector SpawnLocationPoint = CurrentSpawnBox->GetComponentLocation();
				FRotator SpawnRotation = FRotator::ZeroRotator;

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(CurrentEnemyRow->EnemyClass, SpawnLocationPoint, SpawnRotation, SpawnParams);
			}
		}
	}
}




//----------------------------------------------------------------------------

void AQuestTypeA::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		//플레이어인지 체크
		//if (OtherActor->IsA(APlayerCharacter::StaticClass())) // APlayerCharacter는 플레이어 캐릭터 클래스

		// 플레이어가 콜리전 영역에 들어왔을 때 퀘스트 시작
		UE_LOG(LogTemp, Warning, TEXT("퀘스트 시작 콜리전이 감지되었습니다."));
		// 콜리전 이벤트를 비활성화하여 중복 호출 방지
		QuestStartCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// 퀘스트 시작 함수 호출
		StartQuest();
	}
}