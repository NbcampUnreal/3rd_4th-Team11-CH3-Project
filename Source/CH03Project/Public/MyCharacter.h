#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;
class AMyPlayerController;

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
	// Component
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

	// Animation Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireMontage;

	// Effect Valiable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UParticleSystem* ShootHitEffect;

	// State Management EnumClass Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EWeaponState WeaponState;

	// Pointer Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController")
	AMyPlayerController* PlayerController;

	// General Valiable
	float NormalSpeed;
	float RunSpeedMultiplier;
	float RunSpeed;

	// Timer
	FTimerHandle ShootTimerHandle;



public:
	AMyCharacter();

	// Getter, Setter


	// General Function
	void Shoot();



protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

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
