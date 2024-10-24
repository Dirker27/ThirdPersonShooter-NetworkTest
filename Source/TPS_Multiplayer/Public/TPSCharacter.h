// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TPSLocomotionState.h"
#include "TPSCharacterState.h"
#include "TPSCharacterBodyType.h"
#include "TPSWeaponType.h"

#include "TPSCharacter.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString CharacterStateToFString(ETPSCharacterState state) {
		return FString(ETPSCharacterStateToString(state));
	};

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString LocomotionStateToFString(ETPSLocomotionState state) {
		return FString(ETPSLocomotionStateToString(state));
	}

// UE Implementables
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// UE Implementabes
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overrides
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	void FellOutOfWorld(const class UDamageType& dmgType) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EyeSocketName;

	UPROPERTY(BlueprintReadOnly)
	bool IsDebugEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentMaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSCharacterBodyType> CharacterBodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSCharacterState> CurrentCharacterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSCharacterState> PreviousCharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSLocomotionState> CurrentLocomotionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSLocomotionState> PreviousLocomotionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSWeaponType> EquippedWeaponType;

	// True if Crouching OR Prone.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCrouching() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isBoosting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouchInputReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isProneInputReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFiring;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFellOutOfWorld();


	UFUNCTION(BlueprintCallable)
	ETPSLocomotionState EvaluateLocomotionStateForCurrentInput();

	UFUNCTION(BlueprintCallable)
	void ApplyCharacterState(const ETPSCharacterState CharacterState);

	UFUNCTION(BlueprintCallable)
	void ApplyLocomotionState(const ETPSLocomotionState LocomotionState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseSpeedForCharacterState(const ETPSCharacterState CharacterState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpeedModifierForLocomotionState(const ETPSLocomotionState LocomotionState);

	UFUNCTION(BlueprintCallable)
	float UpdateCharacterSpeedForCurrentState();

	UFUNCTION(BlueprintCallable)
	void UpdateInputContextForCurrentState();

	UFUNCTION(BlueprintCallable)
	void ApplyCharacterAttributesForCurrentState();

	UFUNCTION(BlueprintCallable)
	bool IsActionActive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* LineTrace(const UObject* WorldContextObject);
};
