// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "Player/TPSPlayerState.h"
#include "World/TPSCombatLog.h"

#include "TPSGameState.generated.h"

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<int> TeamScores;

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

//~ ==================================================================== ~//
//  COMPONENTS
//~ ==================================================================== ~//
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<UTPSCombatLog> CombatLog;


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
private:

	//UFUNCTION(NetMulticast, Reliable)
	void IndexTeams();

	//UFUNCTION(NetMulticast, Reliable)
	void IndexTeamUnits();

	//UFUNCTION(NetMulticast, Reliable)
	void IndexCharacters();

public:
	//~ Team READ Operations ~//

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




	//~ Command Unit READ Operations ~//

	/*UFUNCTION(BlueprintCallable)
	void CreateTeamUnit(const FTPSUnitID unitId, const FTPSCommandUnitConfiguration unitConfig);*/

	// Find a Command Unit that matches the provided UnitID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandStructure* GetUnit(const FTPSUnitID unitId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(const FTPSUnitID unitId);


	//~ Character READ Operations ~//

	//UFUNCTION(BlueprintCallable)
	//void CreateCharacter(const ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetCharacter(const FGuid characterId);

};
