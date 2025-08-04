#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemInterface.h"
#include "ItemData.h"
#include "BaseItem.generated.h"

// 전방 선언
class USphereComponent;

UCLASS()
class CH03PROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

	// 아이템 인터페이스 함수
	virtual void ShowWidget(AActor* InteractingActor) override;
	virtual void HideWidget(AActor* InteractingActor) override;
	virtual void Interact(AActor* InteractingActor) override;
	virtual void Use(ACharacter* PlayerCharacter) override;

protected:
	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Component")
	USphereComponent* Collision;

	// 아이템 데이터 테이블
	FDataTableRowHandle ItemDataHandle;

	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
