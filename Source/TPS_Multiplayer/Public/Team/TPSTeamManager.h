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

public:
    // Assign the Character to a Team, choosing an appropriate unit to fill.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeam(UTPSCharacterInstance* character, ETPSTeamID team);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(UTPSCharacterInstance* character, FTPSUnitID unit);

    UFUNCTION(BlueprintCallable)
    UTPSTeam* GetTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    UTPSCommandStructure* GetUnit(FTPSUnitID unitId);

private:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCommandStructure* _GetUnit(FTPSUnitID id, UTPSCommandStructure* node);

    UTPSCharacterInstance* NewCharacter(FName name);
};
