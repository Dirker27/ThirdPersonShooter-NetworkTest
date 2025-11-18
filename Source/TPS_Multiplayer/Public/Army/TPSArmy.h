// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Team/TPSCommandStructure.h"
#include "World/TPSSpawnPool.h"

#include "TPSArmy.generated.h"

/**
 * Describes the hierarchy and composition of a given Team.
 *
 * Defines how many Squads-per-Platoon, Units-per-Fireteam, etc along with
 *   appropriate loadouts for each prospective Squad member.
 */
USTRUCT(BlueprintType)
struct FTPSArmySchema
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSHierarchicalLevel> HighestSupportedCommandLevel = ETPSHierarchicalLevel::PLATOON;

    // Defines number of Members directly assigned to each level
    //   ie: Leader units and "HeadQuarters" Squad members
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSHierarchicalLevel>, int> MemberCapacityMap;

    // Defines Squads-per-Platoon, Units-per-Fireteam, etc
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSHierarchicalLevel>, int> SubUnitCapacityMap;

    // Defines what Loadout each SquadRole should spawn with.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TEnumAsByte<ETPSSquadRole>, UTPSEquipmentLoadout*> MemberLoadoutMap;
};

UCLASS(BlueprintType)
class UTPSArmyConfigurationData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FTPSArmySchema Schema;
};




UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmyUpdate);

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSArmyInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSArmyInstance() {};

    // Broadcast Delegate - Team/Character/Player Roster Updated
    UPROPERTY(BlueprintAssignable)
    FArmyUpdate ArmyUpdate;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;

    //~ ==================================================================== ~//
    //  ATTRIBUTES
    //~ ==================================================================== ~//
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
    TEnumAsByte<ETPSTeamID> TeamID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSArmySchema Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TObjectPtr<UTPSCommandStructure> RootUnit;
};
