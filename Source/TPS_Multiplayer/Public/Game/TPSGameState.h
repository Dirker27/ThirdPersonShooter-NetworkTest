// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "GameFramework/GameState.h"

#include "Player/TPSPlayerState.h"
#include "Log/TPSCombatLog.h"
#include "Team/TPSCommandStructure.h"
#include "Team/TPSTeamInstance.h"
#include "Types/TPSBroadcastMessage.h"

#include "TPSGameState.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeamRosterUpdate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatLogUpdate);

/**
 * The TPSGameState of the Game
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
	virtual void OnTick(float deltaTime);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//~ ==================================================================== ~//
//  GAME STATE
//	----------
//	- Timer
//	- Leading Team
//~ ==================================================================== ~//
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeRemainingSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<ETPSTeamID> WinningTeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSBroadcastMessageObject> BroadcastMessage;

//~ ==================================================================== ~//
//  WORLD REGISTRY
//	--------------
//	- Spawned Characters and Items
//	- Teams and Sub-Units
//	- Connected Players
//~ ==================================================================== ~//
public:
	// All Teams instantiated in the World
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSTeamInstance>> Teams;

	// All TeamUnits instantiated in the World (all teams)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSCommandStructure>> TeamUnits;

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
	TMap<int, TObjectPtr<UTPSCommandStructure>> TeamUnitsById;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, TObjectPtr<UTPSCharacterInstance>> CharactersById;

	////////////////////////////////////////////////////////
	// Indexing Operations
private:

	//UFUNCTION(NetMulticast, Reliable)
	void IndexTeams();

	//UFUNCTION(NetMulticast, Reliable)
	void IndexTeamUnits();

	//UFUNCTION(NetMulticast, Reliable)
	void IndexCharacters();

public:
	////////////////////////////////////////////////////////
	// Team CRUD

	/*UFUNCTION(BlueprintCallable)
	void CreateTeam(const ETPSTeamID teamId, const FTPSTeamConfiguration teamConfig);*/

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeamInstance* GetTeam(const ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetActiveTeamMemberCount(const ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalTeamMemberCount(const ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTeamScore(const ETPSTeamID teamId);


	////////////////////////////////////////////////////////
	// Team Sub-Unit CRUD

	/*UFUNCTION(BlueprintCallable)
	void CreateTeamUnit(const FTPSUnitID unitId, const FTPSCommandUnitConfiguration unitConfig);*/

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandStructure* GetUnit(const FTPSUnitID unitId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(const FTPSUnitID unitId);


	////////////////////////////////////////////////////////
	// Character CRUD

	//UFUNCTION(BlueprintCallable)
	//void CreateCharacter(const ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetCharacter(const FGuid characterId);


//~ ==================================================================== ~//
//  COMBAT LOG
//	----------
//	- Kill Events
//	- Objective Updates
//	- Player Join/Drop Events
//~ ==================================================================== ~//
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ClientFeedLength = 5;

	// Full log
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<TObjectPtr<UTPSCombatLogEntry>> LogEntries;

	// Broadcast Delegate - Combat Log has been Updated
	UPROPERTY(BlueprintAssignable)
	FCombatLogUpdate CombatLogUpdate;

public:
	////////////////////////////////////////////////////////
	// Logging Operations
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void LogEvent(UTPSCombatLogEntry* entry);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void BroadcastLogEventToClientFeed(UTPSCombatLogEntry* entry);

	UFUNCTION(BlueprintCallable)
	TArray<UTPSCombatLogEntry*> GetRecentLogEvents(int feedLength) const;
};
