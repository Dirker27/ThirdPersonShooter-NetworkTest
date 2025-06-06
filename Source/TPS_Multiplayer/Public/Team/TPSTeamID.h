// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamID.generated.h"

UENUM(BlueprintType)
enum class ETPSTeamID : uint8
{
	Independent = 0 UMETA(DisplayName = "NONE"), // Independent | Lone Wolves

	Red = 1,
	Blue = 2,
	Green = 3,
	Gold = 4
};
