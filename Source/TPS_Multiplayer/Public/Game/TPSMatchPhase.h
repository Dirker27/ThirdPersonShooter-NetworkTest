// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSMatchPhase.generated.h"

UENUM(BlueprintType)
enum ETPSMatchPhase : int
{
	UnInitialized = 0,
	Setup = 1,
	Engagement = 2,
	Withdrawal = 3,
	Collection = 10,
	Teardown = 99
};
