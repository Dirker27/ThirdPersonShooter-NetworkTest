// (C) ToasterCat Studios 2025

#pragma once

#include "Equipment/TPSEquipmentLoadout.h"

#include "Team/TPSSquadRole.h"

#include "TPSCommandUnitConfiguration.generated.h"

/**
 * Schema and rules for the current level of a command unit
 *
 * TODO: [PC-174] Add Schema definitions for SquadRoles (how many Rifleman, AutoGunners, etc)
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSCommandUnitConfiguration
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumSubCollections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumMembers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSSquadRole, UTPSEquipmentLoadout*> MemberLoadoutMap;

	// TODO: Leader Inheritance Policy, Structure expand/coalesce rules, additional commands, etc
};