// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "Equipment/TPSEquipmentLoadout.h"
#include "GAS/GASAbilitySet.h"
//#include "Team/TPSTeamID.h"

#include "TPSPlayerState.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdatePlayerStateDisplay);

UCLASS()
class TPS_MULTIPLAYER_API ATPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	friend class TPSPlayerController;

public:
	ATPSPlayerState();
	~ATPSPlayerState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

//~ ======================================================================== ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:
	//////////////////////////////////////////////////////
	// Display Widgets

	// Broadcast Delegate
	UPROPERTY(BlueprintAssignable)
	FUpdatePlayerStateDisplay NotifyDisplayWidgets;
private:
	bool ShouldNotify = false;


//~ ======================================================================== ~//
//  ATTRIBUTES
//~ ======================================================================== ~//
public:

	////////////////////////////////////////////////////////
	// State

	/* TODO: [PC-132] Configure Player Loadout + Faction/Team for initial Spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSTeamID TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSFactionID FactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSEquipmentLoadout> SpawnLoadout;
	*/

//~ ======================================================================== ~//
//  ABILITY SYSTEM
//~ ======================================================================== ~//

public:
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	////////////////////////////////////////////////////////
	// Player-Based Abilities
public:
	// Abilities that will be granted to controlled pawns on possession.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UAbilitySet* PlayerAbilitySet { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Input")
	TArray<UInputMappingContext*> CharacterInputMappings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<UInputMappingContext*> InputMappings;


//~ ======================================================================== ~//
//  PUBLIC OPERATIONS
//~ ======================================================================== ~//
public:
	//////////////////////////////////////////////////////
	// Reporting

	// Broadcast Delegate
	UFUNCTION(BlueprintCallable)
	void ReportProjectileHit(FHitResult hit);

};
