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
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSWorldManager> WorldState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSTeamManager> TeamState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTPSCombatLog> CombatLog;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSTeamID, int> TeamScores;


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:

	//- LIVE STATE -//

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTeamScore(ETPSTeamID teamId);


};
