#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Walking         UMETA(DisplayName = "Walking"),
	Running         UMETA(DisplayName = "Running"),
	Crouchinging    UMETA(DisplayName = "Crouching"),
	Jumping			UMETA(DisplayName = "Jumping"),
	Cling			UMETA(DisplayName = "Cling"),
	Shooting		UMETA(DisplayName = "Shooting"),
	Dead			UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Base            UMETA(DisplayName = "Base"),
	Aiming			UMETA(DisplayName = "Aiming")
};

UCLASS()
class CH03PROJECT_API AMyCharacter : public ABaseActor
{
	GENERATED_BODY()

protected:
	// ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	// �ִϸ��̼� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;

	// ���� ���� EnumClass ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponState WeaponState;

	// �Ϲ� ����
	float NormalSpeed;
	float RunSpeedMultiplier;
	float RunSpeed;
	// Ÿ�̸�
	FTimerHandle ShootTimerHandle;



public:
	AMyCharacter();

	// Getter, Setter


	// �Ϲ� �Լ�
	void Shoot();



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& value);

	void StartRun(const FInputActionValue& value);

	void StopRun(const FInputActionValue& value);

	void StartJump(const FInputActionValue& value);

	void StopJump(const FInputActionValue& value);

	void Crouch(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void StartAim(const FInputActionValue& value);

	void StopAim(const FInputActionValue& value);

	void Reload(const FInputActionValue& value);

	void StartShoot(const FInputActionValue& value);

	void StopShoot(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, Category = "Token")
	bool StoreAttackToken(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Token")
	void ReturnAttackToken(int32 Amount);
};
