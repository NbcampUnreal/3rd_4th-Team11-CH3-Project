#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.h"
#include "PickupItem.generated.h"

// 전방 선언
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class CH03PROJECT_API APickupItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	APickupItem();

	// 아이템 인터페이스 함수
	void ShowWidget();
	void HideWidget();
	virtual void Interact() override;

	// 아이템 점수 반환
	UFUNCTION(BlueprintCallable, Category="Item")
	int32 GetItemScore() const;

	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Components")
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UWidgetComponent* InteractUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category="Item")
	UBaseItem* ItemData;

	// 오버랩 관련 함수
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
