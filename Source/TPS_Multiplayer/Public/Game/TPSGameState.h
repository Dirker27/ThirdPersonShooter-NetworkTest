// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSMatchPhase.h"
#include "Containers/Queue.h"
#include "GameFramework/GameState.h"

#include "Player/TPSPlayerState.h"
#include "Log/TPSCombatLog.h"
#include "Army/Unit/TPSCommandUnit.h"
#include "Team/TPSTeamInstance.h"
#include "Types/TPSBroadcastMessage.h"

#include "TPSGameState.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchStateUpdate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeamRosterUpdate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMessageUpdate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLogUpdate);

/**
 * The State of the Game
 *   - Spawned Characters
 *   - Active Teams and Members
 *   - Spawned Weapons/Equipment
 *   - Spawn locations for gear/characters
 */
UCLASS()
class TPS_MULTIPLAYER_API ATPSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATPSGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



//~ ==================================================================== ~//
//  MATCH STATE
//	-----------
//	- Timer
//	- Leading Team
//~ ==================================================================== ~//
protected:
	// Broadcast Delegate - MatchState
	UPROPERTY(BlueprintAssignable)
	FMatchStateUpdate MatchStateUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_MatchPhase)
	TEnumAsByte<ETPSMatchPhase> MatchPhase = UnInitialized;

public:
	UFUNCTION()
	void OnRep_MatchPhase();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdateMatchPhase(ETPSMatchPhase newPhase);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float MatchTimeLimitSeconds = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	double TimeMatchStarted = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	double TimeMatchEnded = -1;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	double GetTimeRemainingSeconds() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetTimeRemainingSecondsAsString() const;

	// Team that has been declared the "winners" for the match
	//   Will only be set when the active phase of the match has concluded.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TObjectPtr<UTPSTeamInstance> WinningTeam = nullptr;



//~ ==================================================================== ~//
//  WORLD REGISTRY
//	--------------
//	- Spawned Characters and Items
//	- Teams and Sub-Units
//	- Connected Players
//~ ==================================================================== ~//
protected:
	// Broadcast Delegate - Team/Character/Player Roster Updated
	UPROPERTY(BlueprintAssignable)
	FTeamRosterUpdate RosterUpdate;

public:
	// All Teams instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSTeamInstance>> Teams;

	// All Army instances instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSArmyInstance>> Armies;

	// All TeamUnits instantiated in the World (all teams)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSCommandUnit>> TeamUnits;

	// All CharacterInstances (not actors) *ever* instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSCharacterInstance>> Characters;

	// All PlayerStates *ever* instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<ATPSPlayerState>> Players;

	////////////////////////////////////////////////////////
	// Local-only indices
	//   (not replicated, but point to rep'd UObjects)
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETPSTeamID>, TObjectPtr<UTPSTeamInstance>> TeamsById;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSArmyID, TObjectPtr<UTPSArmyInstance>> ArmiesById;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSUnitID, TObjectPtr<UTPSCommandUnit>> TeamUnitsById;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSCharacterID, TObjectPtr<UTPSCharacterInstance>> CharactersById;

private:
	void IndexTeams();
	void IndexArmies();
	void IndexTeamUnits();
	void IndexCharacters();

public:
	////////////////////////////////////////////////////////
	// Team CRUD

	// Retrieve a Team Instance that matches the provided TeamID
	//   Will be NULL if Team Instance was not created in the current match.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeamInstance* GetTeam(const ETPSTeamID teamId);

	// Retrieve the number of Active (alive) members on a given team.
	//   Returns ZERO if Team Instance does not exist for provided ArmyID.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetActiveTeamMemberCount(const ETPSTeamID teamId);

	// Retrieves the Total number of team members, alive or dead.
	//   Returns ZERO if Team Instance does not exist for provided ArmyID.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalTeamMemberCount(const ETPSTeamID teamId);

	// Retrieves the current score for a given team.
	//   Returns ZERO if Team Instance does not exist for provided ArmyID.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTeamScore(const ETPSTeamID teamId);


	////////////////////////////////////////////////////////
	// Team Sub-Unit CRUD

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSArmyInstance* GetArmy(const FTPSArmyID armyId);

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandUnit* GetUnit(const FTPSUnitID unitId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(const FTPSUnitID unitId);


	////////////////////////////////////////////////////////
	// Character CRUD

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetCharacter(const FTPSCharacterID characterId);




//~ ==================================================================== ~//
//  COMBAT LOG
//	----------
//	- Kill Events
//	- Objective Updates
//	- Player Join/Drop Events
//~ ==================================================================== ~//
public:
	// Broadcast Delegate - Combat Log has been Updated
	UPROPERTY(BlueprintAssignable)
	FLogUpdate LogUpdate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ClientFeedLength = 5;

	// Full log
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_LogEntries)
	TArray<TObjectPtr<UTPSCombatLogEntry>> LogEntries;

	UFUNCTION()
	void OnRep_LogEntries();

public:
	////////////////////////////////////////////////////////
	// Logging Operations
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LogEvent(UTPSCombatLogEntry* entry);

	UFUNCTION(BlueprintCallable)
	TArray<UTPSCombatLogEntry*> GetRecentLogEvents(int feedLength) const;




//~ ==================================================================== ~//
//  BROADCAST MESSAGE QUEUE
//	-----------------------
//	- Team Objective Updates
//	- Player Death Events
//~ ==================================================================== ~//

protected:
	// Broadcast Delegate - Combat Log has been Updated
	UPROPERTY(BlueprintAssignable)
	FMessageUpdate MessageUpdate;

	// Message that is currently displayed to ALL connected Player UIs
	//   (Match Start, Match End, System Events, etc...)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_BroadcastMessage)
	TObjectPtr<UTPSBroadcastMessageObject> BroadcastMessage;

	UFUNCTION()
	void OnRep_BroadcastMessage();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdateBroadcastMessage(FTPSBroadcastMessage message);
};
