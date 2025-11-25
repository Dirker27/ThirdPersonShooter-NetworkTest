// (C) ToasterCat Studios 2025

#pragma once

#include "TPSTeamInstance.h"
#include "Army/TPSArmyInstanceFactory.h"
#include "Army/Unit/TPSUnitInstanceFactory.h"
#include "Character/TPSCharacterInstanceFactory.h"

#include "TPSTeamInstanceFactory.generated.h"


/**
 * Factory class to create and configure Teams and their SubUnits
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamInstanceFactory : public UActorComponent
{
    friend class ATPSGameMode;

    GENERATED_BODY()

public:
    UTPSTeamInstanceFactory();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSArmyInstanceFactory> ArmyFactory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSUnitInstanceFactory> UnitFactory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterInstanceFactory> CharacterFactory;

//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:
    UTPSTeamInstance* NewTeamInstance(const ETPSTeamID teamId, const FTPSTeamDefinitionData teamConfig, AActor* owner);
    UTPSCommandUnit* NewTeamUnitInstance(const FTPSUnitID teamId, const FTPSUnitSchemaData unitConfig, AActor* owner);


    //~ Team CRUD ~//

    UFUNCTION(BlueprintCallable)
    void CreateTeam(const ETPSTeamID teamId);

    UFUNCTION(BlueprintCallable)
    void ConfigureTeam(const ETPSTeamID teamId, const FTPSTeamDefinitionData definition);

    UFUNCTION(BlueprintCallable)
    void PopulateTeam(const ETPSTeamID teamId, TArray<UTPSCharacterInstance*> roster);

    // Assign the Character to a specific Unit within a Team.
    UFUNCTION(BlueprintCallable)
    void AssignCharacterToTeamUnit(const FTPSUnitID unitId, UTPSCharacterInstance* character);

private:
    ATPSGameState* State() const;

    void _ConfigureArmy(UTPSArmyInstance* army, FTPSArmyDefinitionData data);
    void _ConfigureUnit(UTPSCommandUnit* node, FTPSUnitSchemaData schema, UTPSArmyInstance* army);
    void _PopulateUnit(UTPSCommandUnit* node);
    UTPSCharacterInstance* _NewCharacter();
};
