// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "Team/TPSTeamManager.h"
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


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSTeam* GetTeam(ETPSTeamID teamId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCommandStructure* GetUnit(FTPSUnitID id);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTPSCharacterInstance* GetUnitLeader(FTPSUnitID id);


};
