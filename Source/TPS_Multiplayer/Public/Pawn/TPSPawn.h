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
public:
	// Target Rotation - Derived from Target Location (Replicated to peer clients)
	// TODO: DELETE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn|State|Input", Replicated)
	FRotator TargetLookRotation;

	//////////////////////////////////////////////////////
	// Targeting
	//
	// Targeted Location(s) / Actor(s)
	UPROPERTY(VisibleAnywhere, Category = "TPSCharacter|Input|Targeting", Replicated)
	FTPSTargetInfo TargetInfo;
	//
	// Target Lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TPSPawn|State|Targeting")
	bool IsLockedToTarget = false;


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
