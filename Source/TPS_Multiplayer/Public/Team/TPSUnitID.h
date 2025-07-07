// (C) ToasterCat Studios 2025

#pragma once

//#include "TPSTeamID.h"
#include "TPSTeamID.h"
#include "Components/ActorComponent.h"
#include "Model/TPSHierarchicalCollectionLevel.h"

#include "TPSUnitID.generated.h"

/**
 * Identifier to specify an exact unit in a given Team.
 *
 * Will be unique for each team, but may be duplicated across multiple teams.
 *   ie: there can be an 'Abel Squad' on the other Team as well as your own.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FUnitID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSTeamID> TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSHierarchicalLevel> UnitLevel;

	// Display Name for the Unit
	//   ('Easy' Company, 'Abel' Squad, etc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UnitAlias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETPSHierarchicalLevel>, int> ParentHierarchyMap;
};

