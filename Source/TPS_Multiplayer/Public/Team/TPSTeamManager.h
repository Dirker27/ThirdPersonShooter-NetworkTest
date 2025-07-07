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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<ETPSTeamID, UTPSTeam*> ActiveTeams;

    UFUNCTION(BlueprintCallable)
    void CreateTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void ConfigureTeam(ETPSTeamID teamId, int numPlatoons, int numSquadsPerPlatoon);

    // Assign the Character to a Team, choosing an appropriate unit to fill.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeam(ATPSCharacter* character, ETPSTeamID team);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(ATPSCharacter* character, FUnitID unit);


    UFUNCTION(BlueprintCallable)
    UTPSTeam* GetTeam(ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    UTPSCommandStructure* GetUnit(FUnitID unitId);
};