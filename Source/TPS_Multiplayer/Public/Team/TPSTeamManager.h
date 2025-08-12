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

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<ETPSTeamID, TObjectPtr<UTPSTeam>> ActiveTeams;

public:
    UFUNCTION(BlueprintCallable)
    void CreateTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void ConfigureTeam(ETPSTeamID teamId, FTPSTeamConfiguration configuration);

    UFUNCTION(BlueprintCallable)
    void PopulateTeam(ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster);



public:
    UFUNCTION(BlueprintCallable)
    void ActivateCharacter(UTPSCharacterInstance* instance);

    // Assign the Character to a Team, choosing an appropriate unit to fill.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeam(ETPSTeamID team, UTPSCharacterInstance* character);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(FTPSUnitID unit, UTPSCharacterInstance* character);

    UFUNCTION(BlueprintCallable)
    UTPSTeam* GetTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    UTPSCommandStructure* GetUnit(FTPSUnitID unitId);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetUnitLeader(FTPSUnitID id);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetActiveTeamMemberCount(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetTotalTeamMemberCount(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    int CountUnitMembers(FTPSUnitID unitId);


private:
    void _ConfigureUnit(UTPSCommandStructure* node, FTPSTeamConfiguration configuration,
        ETPSHierarchicalLevel level);
    void _PopulateUnit(UTPSCommandStructure* node);
    int _CountUnitMembers(UTPSHierarchicalCollection* node);
    UTPSCharacterInstance* _NewCharacter();
};
