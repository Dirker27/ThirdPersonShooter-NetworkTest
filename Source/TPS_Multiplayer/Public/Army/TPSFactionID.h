// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSFactionID.generated.h"


/**
 * GAME FACTIONS - Who a player's Army is fighting *for*.
 *
 * Delineated from "Teams" and "Armies" to allow for Multi-Faction coalitions and Intra-Faction conflicts.
 *   An Army will have one faction it is allied to. Teams may have multiple armies of differing allegiances.
 *
 * See Design Doc for Faction Details: https://www.notion.so/Factions-28f37b4ab2e8808bb08cfa9e9c846db3
 */
UENUM(BlueprintType)
enum ETPSFactionID : int
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
	// Northern Union [NU]
	Union = 13 UMETA(DisplayName = "Northern Union"),


	//- "Red" Empire ------------------------------------------------=
	//
	Empire = 20,
	// American (Con)Federation [AF]
	Federation = 21 UMETA(DisplayName = "American Federation"),


	//- Peacekeeper Coalition ---------------------------------------=
	//
	Peacekeeper = 30,
	//
	UN = 31,
	//
	TPAC = 32,
	//
	EU = 33,


	//- Independent States ------------------------------------------=
	//
	Appalachia = 40,
	// First Nations Alliance [FNA]
	FirstNations = 41,
	// Monorai / "New Zion" [NZ]
	Mormon = 42,
	// Creole Republic
	Creole = 43,
	// Lincoln (WA/OR) [LN]
	Lincoln = 44,
	// Republic of Texas [TX]
	Texas = 45,
	// Pueblo
	Pueblo = 46,
	// Orange Counties [OC]
	OrangeCounty = 47,
	// Golden Hills [GH]
	GoldenHills = 48
};