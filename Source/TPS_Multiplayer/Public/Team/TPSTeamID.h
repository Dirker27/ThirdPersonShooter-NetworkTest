// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamID.generated.h"

// TODO: Replace w/ UnitID?
UENUM(BlueprintType)
enum class ETPSTeamID : uint8
{
	Independent = 0 UMETA(DisplayName = "NONE"), // Independent | Lone Wolves

	Red = 1,
	Blue = 2,
	Green = 3,
	Gold = 4
};


static FString TPSTeamIdToString(ETPSTeamID id)
{
	switch (id)
	{
	case ETPSTeamID::Red:
		return "Red";
	case ETPSTeamID::Blue:
		return "Blue";
	case ETPSTeamID::Green:
		return "Green";
	case ETPSTeamID::Gold:
		return "Gold";
	default:
		return "TEAM";
	}
}
