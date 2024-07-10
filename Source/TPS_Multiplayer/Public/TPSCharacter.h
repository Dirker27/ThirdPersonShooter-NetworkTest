// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UFUNCTION(BlueprintImplementableEvent)
	void OnFellOutOfWorld();

	// CANNOT REFERENCE BP ENUMS FROM C++
	UFUNCTION(BlueprintCallable)
	float GetBaseSpeedForCharacterState(const ETPSCharacterState CharacterState) const;
};
