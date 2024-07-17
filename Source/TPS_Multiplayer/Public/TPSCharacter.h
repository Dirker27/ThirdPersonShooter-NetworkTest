// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "EnhancedInput/Public/EnhancedInputPlatformSettings.h"

#include "TPSLocomotionState.h"
#include "TPSCharacterState.h"

#include "TPSCharacter.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	void FellOutOfWorld(const class UDamageType& dmgType) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EyeSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isBoosting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFiring;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFellOutOfWorld();

	UFUNCTION(BlueprintCallable)
	float GetBaseSpeedForCharacterState(const ETPSCharacterState CharacterState) const;

	UFUNCTION(BlueprintCallable)
	AActor* LineTrace(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	void UpdateInputContextToState();
};
