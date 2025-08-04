#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;


UCLASS()
class CH03PROJECT_API AMyCharacter : public ABaseActor
{
	GENERATED_BODY()

protected:
	// ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	// ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;

	// �Ϲ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsShooting;

	// Ÿ�̸�
	FTimerHandle ShootTimerHandle;



public:
	AMyCharacter();

	// Getter, Setter
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetbIsShooting();

	// �Ϲ� �Լ�
	void Shoot();



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void StartShoot(const FInputActionValue& value);

	void StopShoot(const FInputActionValue& value);
};
