// (C) ToasterCat Studios 2025

#pragma once

#include "TPSTeamConfiguration.generated.h"

/**
 * Describes the hierarchy and composition of a given Team.
 *
 * Defines how many Squads-per-Platoon, Units-per-Fireteam, etc along with
 *   appropriate loadouts for each prospective Squad member.
 */
USTRUCT(BlueprintType)
struct FTPSTeamConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSHierarchicalLevel HighestSupportedCommandLevel = ETPSHierarchicalLevel::PLATOON;

	// Defines number of Members directly assigned to each level
	//   ie: Leader units and "HeadQuarters" Squad members
	//   (TODO: Migrate to "ForceHierarchy"/"TeamSchema" object)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> MemberCapacityMap;

	// Defines Squads-per-Platoon, Units-per-Fireteam, etc
	//   (TODO: Migrate to "ForceHierarchy"/"TeamSchema" object)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> SubUnitCapacityMap;

	// Defines what Loadout each SquadRole should spawn with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSSquadRole, UTPSEquipmentLoadout*> MemberLoadoutMap;
};




UCLASS(BlueprintType)
class UTPSTeamConfigurationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSTeamConfiguration Configuration;
};