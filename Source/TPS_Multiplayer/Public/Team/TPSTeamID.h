// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamID.generated.h"

static const int TEAM_SLOT_SIZE = 5;

UENUM(BlueprintType)
enum ETPSTeamID : int
{
	Independent = 0 UMETA(DisplayName = "NONE"), // Independent | Lone Wolves

	Red = 1,
	Blue = 2,
	Green = 3,
	Gold = 4
};

static FColor TPSTeamIdToColor(ETPSTeamID id)
{
	switch (id)
	{
	case ETPSTeamID::Red:
		return FColor(255, 25, 25);
	case ETPSTeamID::Blue:
		return FColor(0, 255, 255);
	case ETPSTeamID::Green:
		return FColor(24, 255, 96);
	case ETPSTeamID::Gold:
		return FColor::Orange;
	default:
		return FColor::White;
	}
}


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
		return "[TEAM]";
	}
}
