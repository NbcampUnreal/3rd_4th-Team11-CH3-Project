#include "MoveTriggerArea.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "AI/EnemyAIController.h"

AMoveTriggerArea::AMoveTriggerArea()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AMoveTriggerArea::BeginPlay()
{
    EnemyTargetingVolume->OnActorBeginOverlap.AddDynamic(this, &AMoveTriggerArea::OnVolumeBeginOverlap);
}

void AMoveTriggerArea::OnVolumeBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어가 타겟팅 볼륨에 진입했습니다."));

		if (EnemyStartingVolume)
		{
			TArray<AActor*> OverlappingActors;
			
			EnemyStartingVolume->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor && Actor->ActorHasTag("Enemy"))
                {
                    // AActor를 먼저 ABaseEnemy 타입으로 형 변환
                    ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Actor);
                    if (BaseEnemy)
                    {
                        // 이제 GetController()를 안전하게 호출할 수 있음
                        AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(BaseEnemy->GetController());
                        if (EnemyAIController)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("적 AI 컨트롤러가 유효합니다."));
                            EnemyAIController->TryForceTargetPlayer();
                        }
                    }
                }
            }
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EnemyStartingVolume이 유효하지 않습니다. 블루프린트에서 할당해주세요."));
		}
	}
}

