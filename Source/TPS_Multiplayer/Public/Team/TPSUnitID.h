// (C) ToasterCat Studios 2025

#pragma once

#include "Components/ActorComponent.h"

#include "Model/TPSHierarchicalCollectionLevel.h"
#include "Team/TPSTeamID.h"

#include "TPSUnitID.generated.h"

/**
 * Identifier to specify an exact unit in a given Team.
 *
 * Will be unique for each team, but may be duplicated across multiple teams.
 *   ie: there can be an 'Abel Squad' on the other Team as well as your own.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSTeamID TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSHierarchicalLevel UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> Hierarchy;
};

