// (C) ToasterCat Studios 2025

#pragma once

#include "Components/ActorComponent.h"

#include "TPSTeamInstance.h"

#include "TPSTeamInstanceFactory.generated.h"


/**
 * Factory class to create and configure Teams and their SubUnits
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamInstanceFactory : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSTeamInstanceFactory();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//
private:
    /* STUB */

//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:

    static UTPSTeamInstance* NewTeamInstance(const ETPSTeamID teamId, const FTPSTeamConfiguration teamConfig, AActor* owner);
    static UTPSCommandStructure* NewTeamUnitInstance(const FTPSUnitID teamId, const FTPSCommandUnitConfiguration unitConfig, AActor* owner);


    //~ Team CRUD ~//

    UFUNCTION(BlueprintCallable)
    void CreateTeam(const ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void ConfigureTeam(const ETPSTeamID teamId, const FTPSTeamConfiguration configuration);

    UFUNCTION(BlueprintCallable)
    void PopulateTeam(const ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster);

    // Assign the Character to a Team, choosing an appropriate unit to fill.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeam(const ETPSTeamID teamId, UTPSCharacterInstance* character);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(const FTPSUnitID unitId, UTPSCharacterInstance* character);


    //~ Team Member / Character CRUD ~//

    UFUNCTION(BlueprintCallable)
    void ActivateCharacter(UTPSCharacterInstance* instance);

private:
    void _ConfigureUnit(UTPSCommandStructure* node, FTPSTeamConfiguration configuration,
        ETPSHierarchicalLevel level);
    void _PopulateUnit(UTPSCommandStructure* node);
    UTPSCharacterInstance* _NewCharacter();
};
