#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedIsAiming, bool, bIsAiming);

class UCameraComponent;
struct FInputActionValue;
class AMyPlayerController;
class UBaseStatComponent;
class UDamageComponent;
class ABaseWeapon;

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Walking         UMETA(DisplayName = "Walking"),
	Running         UMETA(DisplayName = "Running")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Crouching		UMETA(DisplayName = "Crouching"),
	Jumping			UMETA(DisplayName = "Jumping"),
	Cling			UMETA(DisplayName = "Cling"),
	Dead			UMETA(DisplayName = "Dead")
};

UCLASS()
class CH03PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBaseStatComponent* BaseStatComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UDamageComponent* DamageComp;

	// Deligate
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnChangedIsAiming OnChangedIsAiming;

protected:
	// Animation Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* CharacterAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* WeaponAnimInstance;

	// State Management EnumClass Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EMoveState MoveState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EActionState ActionState;

	// State Valiable
	bool bIsCrouching;
	bool bIsAiming;

	// TSubclassOf Valiable
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ABaseWeapon> WeaponClass;

	// Pointer Valiable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerController")
	AMyPlayerController* PlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABaseWeapon* EquippedWeapon;

	// General Valiable
	float NormalSpeed;
	float RunSpeedMultiplier;
	float RunSpeed;

	// Timer
	FTimerHandle AttackTimerHandle;
	FTimerHandle ReloadTimerHandle;

public:
	AMyCharacter();

	// Getter, Setter
	AMyPlayerController* GetMyPlayerController() const;
	EMoveState GetMoveState() const;
	EActionState GetActionState() const;
	UAnimInstance* GetCharacterAnimInstance() const;
	UAnimInstance* GetWeaponAnimInstance() const;

	// General Function
	void Attack();

	void EndReload();

	virtual void Landed(const FHitResult& Hit) override;

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

	void StartAttack(const FInputActionValue& value);

	void StopAttack(const FInputActionValue& value);

	void OnInteract(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Token")
	bool StoreAttackToken(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Token")
	void ReturnAttackToken(int32 Amount);

};
