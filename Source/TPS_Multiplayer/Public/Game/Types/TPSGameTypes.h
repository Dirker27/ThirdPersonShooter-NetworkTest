// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSGameTypes.generated.h"

UENUM(BlueprintType)
enum ETPSGameModeObjective : int
{
	DEATHMATCH = 0 UMETA(DisplayName = "Deathmatch"),
	EVACUATION = 1 UMETA(DisplayName = "Evacuation"),
	RAID = 2 UMETA(DisplayName = "Raid"),

	FREE_ROAM = 100
};

UENUM(BlueprintType)
enum ETPSGameModeWinCondition : int
{
	Points,
	Extermination,
	TimeOut
};
