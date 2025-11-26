// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "TPSPlayerID.h"

#include "GAS/GASAbilitySet.h"
#include "Team/TPSTeamID.h"

#include "TPSPlayerState.generated.h"


class UTPSArmyInstance;
class UTPSTeamInstance;


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerStateUpdate);

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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

//~ ======================================================================== ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:
	//////////////////////////////////////////////////////
	// Display Widgets

	// Broadcast Delegate
	UPROPERTY(BlueprintAssignable)
	FPlayerStateUpdate PlayerStateUpdate;


//~ ======================================================================== ~//
//  ATTRIBUTES
//~ ======================================================================== ~//
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FTPSPlayerID ID;

	////////////////////////////////////////////////////////
	// Team Affiliations

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ETPSTeamID> TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ETPSFactionID> FactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTPSArmyID Army;*/


//~ ======================================================================== ~//
//  LIVE STATE
//~ ======================================================================== ~//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> FocusActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> TargetActor;

protected:
	// Assigned Army
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSArmyInstance> AssignedArmy;

	// Assigned Team
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TWeakObjectPtr<UTPSTeamInstance> AssignedTeam;

public:
	UFUNCTION(BlueprintCallable)
	void AssignToTeam(UTPSTeamInstance* team);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeamInstance* GetAssignedTeam() { return AssignedTeam.Get(); }

	// Assigned Team ID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ETPSTeamID GetAssignedTeamID() const;


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
