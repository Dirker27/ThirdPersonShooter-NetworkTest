// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "Team/TPSTeamManager.h"
#include "World/TPSCombatLog.h"
#include "World/TPSWorldManager.h"

#include "TPSGameState.generated.h"

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

//~ ==================================================================== ~//
//  COMPONENTS
//~ ==================================================================== ~//
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSWorldManager> WorldState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSTeamManager> TeamState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSCombatLog> CombatLog;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSTeamID, int> TeamScores;


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:

	//- LIVE STATE -//

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTeamScore(ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTeamUnitCount(ETPSTeamID teamId);

	//- TEAM CRUD -//

	UFUNCTION(BlueprintCallable)
	void CreateTeam(ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable)
	void ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration);

	UFUNCTION(BlueprintCallable)
	void PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeam* GetTeam(ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandStructure* GetUnit(FTPSUnitID id);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(FTPSUnitID id);


};
