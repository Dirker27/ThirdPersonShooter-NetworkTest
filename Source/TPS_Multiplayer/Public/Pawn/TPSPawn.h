// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/DefaultPawn.h"

#include "GAS/TPSAbilitySystemComponent.h"

#include "TPSPawn.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSPawn : public ADefaultPawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPawn();
	~ATPSPawn();

	// UE Implementables
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif


//~ ======================================================================== ~//
//  STATE
//~ ======================================================================== ~//
protected:
	// Should Target Location drive Target Rotation?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input")
	bool IsTargetingLocation;
	// Target Location - Provided by LOCAL Controller (Not Replicated)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input")
	FVector TargetLookLocation;
	// Current Look Location - Iterps to TargetLocation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input")
	FVector CurrentLookLocation;
	// Target Rotation - Derived from Target Location (Replicated to peer clients)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSCharacter|State|Input", Replicated)
	FRotator TargetLookRotation;


//~ ======================================================================== ~//
//  PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:
	UFUNCTION(BlueprintCallable)
	void SetTargetLocation(FVector targetLocation) { TargetLookLocation = targetLocation; }


//~ ======================================================================== ~//
//  ABILITY SYSTEM
//~ ======================================================================== ~//
protected:
	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	UTPSAbilitySystemComponent* AbilitySystem { nullptr };

public:
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_PlayerState() override;
};
