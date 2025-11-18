// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "Army/TPSArmyID.h"
#include "Equipment/TPSEquipmentLoadout.h"
#include "GAS/GASAbilitySet.h"
#include "Team/TPSTeamID.h"
#include "Team/Types/TPSFactionID.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

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
	// Team Affiliations

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ETPSTeamID> TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ETPSFactionID> FactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTPSArmyID Army;

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
