// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSFactionID.generated.h"


// GAME FACTIONS
//   Delineated from "Teams" to allow for Multi-Faction teams (alliances) and Intra-Faction conflicts.
UENUM(BlueprintType)
enum class ETPSFactionID : uint8
{
	Unaffiliated = 0 UMETA(DisplayName = "Unaffiliated"),

	//- "Blue" Alliance ---------------------------------------------=
	//
	Alliance = 10,
	// Cascadian Coastal Republic [CCR]
	Cascadia = 11 UMETA(DisplayName = "Cascadian Coastal Replublic"),
	//
	// Greater Bay [GB]
	Bay = 12 UMETA(DisplayName = "Geater Bay"),


	//- "Red" Empire ------------------------------------------------=
	//
	Empire = 20,
	// American (Con)Federation [AF]
	Federation = 21 UMETA(DisplayName = "American Federation"),


	//- Peacekeeper Coalition ---------------------------------------=
	//
	Peacekeeper = 30,
	//
	UnitedNations = 31,
	//
	China = 31,
	//
	Russia = 32,
	//
	EuropeanUnion = 33,


	//- Independent States ------------------------------------------=
	//
	Independent = 40,
	// Monorai / "New Zion" [NZ]
	Mormon = 42,
	// Republic of Texas [TX]
	Texas = 45,
};