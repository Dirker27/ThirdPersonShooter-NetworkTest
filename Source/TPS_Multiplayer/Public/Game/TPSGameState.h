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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInstanceRegistryUpdate);

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



//~ ======================================================================= ~//
//  WORLD REGISTRY
//	--------------
//	- Spawned Characters and Items
//	- Teams and Sub-Units
//	- Connected Players
//~ ======================================================================= ~//
protected:
	// Broadcast Delegate - Team/Character/Player Roster Updated
	UPROPERTY(BlueprintAssignable)
	FInstanceRegistryUpdate InstanceRegistryUpdate;


	////////////////////////////////////////////////////////
	// TEAM Instances
public:
	// All Teams instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Teams)
	TArray<TObjectPtr<UTPSTeamInstance>> Teams;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETPSTeamID>, TObjectPtr<UTPSTeamInstance>> TeamsById;
	UFUNCTION()
	void OnRep_Teams();
private:
	void IndexLocalTeamIds();


	////////////////////////////////////////////////////////
	// ARMY Instances
public:
	// All Army instances instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Armies)
	TArray<TObjectPtr<UTPSArmyInstance>> Armies;
protected:
	// Local-Only Index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSArmyID, TObjectPtr<UTPSArmyInstance>> ArmiesById;
	UFUNCTION()
	void OnRep_Armies();
private:
	void IndexLocalArmyIds();


	////////////////////////////////////////////////////////
	// UNIT Instances
public:
	// All Units instantiated in the World (all teams and armies)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Units)
	TArray<TObjectPtr<UTPSCommandUnit>> Units;
protected:
	// Local-Only Index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSUnitID, TObjectPtr<UTPSCommandUnit>> UnitsById;
	UFUNCTION()
	void OnRep_Units();
private:
	void IndexLocalUnitIds();


	////////////////////////////////////////////////////////
	// CHARACTER Instances
public:
	// All CharacterInstances (not actors) *ever* instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Characters)
	TArray<TObjectPtr<UTPSCharacterInstance>> Characters;
protected:
	// Local-Only Index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FTPSCharacterID, TObjectPtr<UTPSCharacterInstance>> CharactersById;
	UFUNCTION()
	void OnRep_Characters();
private:
	void IndexLocalCharacterIds();

	////////////////////////////////////////////////////////
	// PLAYER Instances
public:
	// All PlayerStates *ever* instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Players)
	TArray<TObjectPtr<ATPSPlayerState>> Players;
protected:
	// Local-Only Index
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<int32, TObjectPtr<ATPSPlayerState>> PlayersById;
	UFUNCTION()
	void OnRep_Players();
private:
	void IndexLocalPlayerIds();




public:
	////////////////////////////////////////////////////////
	// Team CRUD

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RegisterTeam(UTPSTeamInstance* team);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnRegisterTeam(const ETPSTeamID tId);

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
	// Army CRUD

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RegisterArmy(UTPSArmyInstance* army);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnRegisterArmy(const FTPSArmyID aId);

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSArmyInstance* GetArmy(const FTPSArmyID armyId);


	////////////////////////////////////////////////////////
	// Unit CRUD

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RegisterUnit(UTPSCommandUnit* unit);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnRegisterUnit(const FTPSUnitID uId);

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandUnit* GetUnit(const FTPSUnitID unitId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(const FTPSUnitID unitId);


	////////////////////////////////////////////////////////
	// Character CRUD

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RegisterCharacter(UTPSCharacterInstance* character);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnRegisterCharacter(const FTPSCharacterID cId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetCharacter(const FTPSCharacterID characterId);


	////////////////////////////////////////////////////////
	// Player CRUD

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RegisterPlayer(ATPSPlayerState* player);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnRegisterPlayer(const int pId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ATPSPlayerState* GetPlayer(const int pId);


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
