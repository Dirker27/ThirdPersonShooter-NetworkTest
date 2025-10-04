// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSMatchPhase.generated.h"

UENUM(BlueprintType)
enum ETPSMatchPhase : int
{
	UnInitialized  = 00,

	// Instantiate instances and prepare teams
	Initialization = 01,

	// Prepare the map and spawn-in actors for instances
	Setup          = 02,



	//~ Active Phases ~//



	// The actual shooting phase
	Engagement     = 10,

	// The losers retreat, the winners rout and consolidate
	Withdrawal     = 20,

	// Winners collect loot
	Collection     = 30,

	// Disconnect clients and clean up resources
	Teardown       = 99
};
