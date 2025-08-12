#include "QuestTypeA.h"
#include "GameModePlay.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "SubTextDataRow.h"
#include "GameStatePlay.h"
#include "SpawnAreaActor.h"
#include "Items/PickupItem.h"
#include "BaseEnemy.h"

AQuestTypeA::AQuestTypeA()
{	
	PrimaryActorTick.bCanEverTick = false;
	ProgressStage = 0;
	FirstAreaTargetKillCount = 0;
	
	QuestStartCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("QuestStartCollision"));
}

void AQuestTypeA::BeginPlay()
{
	Super::BeginPlay();
	GameModePlays = Cast<AGameModePlay>(UGameplayStatics::GetGameMode(GetWorld()));
	
	SetSubTexts();
	AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameStatePlay)
	{
		GameStatePlay->OnKillCountChanged.AddDynamic(this, &AQuestTypeA::UpdateKillCount);
	}
	ProgressStarter();	//생성주기 때문에 문제가 될 수도 있다. 발생하면 변경.

	QuestStartCollision->OnComponentBeginOverlap.AddDynamic(this, &AQuestTypeA::OnOverlapBegin);


	//타이머 설치해서 10초뒤에 DestroyAllEnemies() 실행
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AQuestTypeA::DestroyAllEnemies, 10.0f, false);
}

void AQuestTypeA::SetSubTexts()
{
	if (!SubTextsDataTable)
	{
		return;		
	}
	SubTexts.Empty();

	TArray<FSubTextDataRow*> AllRows;

	SubTextsDataTable->GetAllRows<FSubTextDataRow>(TEXT("SubText"), AllRows);

	for (const FSubTextDataRow* Row : AllRows)
	{
		if (Row)
		{
			SubTexts.Add(Row->SubText);
		}
	}
}



void AQuestTypeA::ProgressStarter()
{
	switch (ProgressStage)
	{
	case 0:
		Progress00();
		break;
	case 1:
		Progress01();
		break;
	case 2:
		Progress02();
		break;
	case 3:
		Progress03();
		break;
	case 4:
		Progress04();
		break;
	case 5:	
		Progress05();
		break;
	}
}

void AQuestTypeA::Progress00()	//시작
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 0단계"));
	GameModePlays->SetGameStatePlay();
	GameModePlays->SetMissionText(SubTexts[0]);
}


void AQuestTypeA::Progress01()	//문지남 키얻으쇼
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 1단계"));
	
	AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameStatePlay)
	{
		StartKillCount = GameStatePlay->GetKillCount();
	}
	FString MissionText = FString::Printf(TEXT("%s (%d / %d)"), *SubTexts[1], 0, FirstAreaTargetKillCount);
	GameModePlays->SetMissionText(MissionText);
}

void AQuestTypeA::Progress02()	//키얻음 문여쇼
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 2단계"));
	GameModePlays->SetMissionText(SubTexts[2]);
}
void AQuestTypeA::Progress03()	//키소모 키 또 얻으쇼
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 3단계"));
	GameModePlays->SetMissionText(SubTexts[3]);
}
void AQuestTypeA::Progress04()	//키얻음 문여쇼
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 4단계"));
	GameModePlays->SetMissionText(SubTexts[2]);

	//적 스폰 필요
	SpawnEnemy();
}
void AQuestTypeA::Progress05()	//문 오픈대기 : 시간시간
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 5단계"));
	GameModePlays->SetMissionText(SubTexts[4]);
}
void AQuestTypeA::Progress06()	//문 열림
{
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 5단계"));
	GameModePlays->SetMissionText(SubTexts[4]);
}



void AQuestTypeA::UpdateKillCount(int32 Points)
{
	if (ProgressStage == 1)
	{
		
		FString MissionText = FString::Printf(TEXT("%s (%d / %d)"), *SubTexts[1], (Points - StartKillCount), FirstAreaTargetKillCount);
		GameModePlays->SetMissionText(MissionText);

		//드랍방식
		UE_LOG(LogTemp, Warning, TEXT("킬 카운트 업데이트: %d"), Points - StartKillCount);
		if (FirstAreaTargetKillCount == Points - StartKillCount)
		{	
			UE_LOG(LogTemp, Warning, TEXT("아이템스폰"));
			AGameStatePlay* GameStatePlay = Cast<AGameStatePlay>(UGameplayStatics::GetGameState(GetWorld()));
			if (!GameStatePlay)
			{
				UE_LOG(LogTemp, Warning, TEXT("GameStatePlay가 유효하지 않습니다."));
				return;
			}
			APickupItem* SpawnedItem = GetWorld()->SpawnActor<APickupItem>(KeyItem, GameStatePlay->GetLastLocation(), FRotator::ZeroRotator);
			if (SpawnedItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("아이템이 성공적으로 스폰되었습니다."));
				GameModePlays->SetMissionText(SubTexts[5]);	//키 주으쇼
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("아이템 스폰 실패"));
			}
		}
	}
}

void AQuestTypeA::UpdateKeyItemCount()
{
	UE_LOG(LogTemp, Warning, TEXT("키 아이템 수"));
	ProgressStage++;
	ProgressStarter();
}

//구역2에서 키를 얻으면 아래 함수를 호출
void AQuestTypeA::SpawnEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("몬스터가 소환이 시작되었습니다."));

	

	// 스폰 위치에서 몬스터 소환
	int32 MaxSpawnCount = FMath::Min(SpawnLocation.Num(), SpawnDataArray.Num());

	for (int32 i = 0; i < MaxSpawnCount; ++i)
	{
		// 해당 인덱스의 SpawnLocation 박스가 유효한지 확인
		ASpawnAreaActor* CurrentSpawnArea = SpawnLocation[i];
		if (!CurrentSpawnArea)
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
				FVector SpawnLocationPoint = CurrentSpawnArea->GetActorLocation();
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
	UE_LOG(LogTemp, Warning, TEXT("충돌 시작"));
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("충돌 시작 1단계 패스"));
		//플레이어인지 체크
		if (!OtherActor->ActorHasTag("Player"))
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("충돌 시작 2단계 패스"));

		if (ProgressStage == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("퀘스트 시작 콜리전이 감지되었습니다."));
			QuestStartCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ProgressStage++;
			ProgressStarter();
		}
	}
}


void AQuestTypeA::DestroyAllEnemies()
{
	UE_LOG(LogTemp, Warning, TEXT("모든 적을 파괴합니다."));
	if (EnemyAliveVolume)
	{	
		UE_LOG(LogTemp, Warning, TEXT("EnemyAliveVolume이 유효합니다."));
		TArray<AActor*> FoundEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundEnemies);

		for (AActor* EnemyActor : FoundEnemies)
		{
			UE_LOG(LogTemp, Warning, TEXT("적 액터 발견: %s"), *EnemyActor->GetName());
			if (!EnemyAliveVolume->IsOverlappingActor(EnemyActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("적 액터가 EnemyAliveVolume과 겹치지 않습니다. 파괴합니다."));
				EnemyActor->Destroy();
			}
		}
	}
}