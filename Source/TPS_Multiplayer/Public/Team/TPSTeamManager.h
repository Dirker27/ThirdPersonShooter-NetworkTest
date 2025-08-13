// (C) ToasterCat Studios 2025

#pragma once

#include "Components/ActorComponent.h"

#include "TPSTeam.h"

#include "TPSTeamManager.generated.h"





/**
 * Responsible for spawning and tracking loose objects in the world.
 *   (equipment, dead characters, world objects, etc)
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSTeamManager();

//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)//, Replicated)
    TMap<ETPSTeamID, TObjectPtr<UTPSTeam>> ActiveTeams;


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:

    //~ Team CRUD ~//

    UFUNCTION(BlueprintCallable)
    void CreateTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration);

    UFUNCTION(BlueprintCallable)
    UTPSTeam* GetTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster);

    // Assign the Character to a Team, choosing an appropriate unit to fill.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeam(ETPSTeamID team, UTPSCharacterInstance* character);

    //~ Command Unit CRUD ~//

    // Find a Command Unit that matches the provided UnitID
    UFUNCTION(BlueprintCallable)
    UTPSCommandStructure* GetUnit(FTPSUnitID unitId);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetUnitLeader(FTPSUnitID id);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(FTPSUnitID unit, UTPSCharacterInstance* character);

    // Perform a deep-scan count of members under the unit's umbrella.
    UFUNCTION(BlueprintCallable)
    int CountUnitMembers(FTPSUnitID unitId);

    //~ Team Member / Character CRUD ~//

    UFUNCTION(BlueprintCallable)
    void ActivateCharacter(UTPSCharacterInstance* instance);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetActiveTeamMemberCount(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetTotalTeamMemberCount(ETPSTeamID teamId);

private:
    void _ConfigureUnit(UTPSCommandStructure* node, FTPSTeamConfiguration configuration,
        ETPSHierarchicalLevel level);
    void _PopulateUnit(UTPSCommandStructure* node);
    int _CountUnitMembers(UTPSHierarchicalCollection* node);
    UTPSCharacterInstance* _NewCharacter();
};
