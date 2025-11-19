// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSUnitLevel.generated.h"

UENUM(BlueprintType)
enum ETPSUnitLevel : int
{
	UNIT = 0,
	FIRE_TEAM = 1,
	SQUAD = 2,
	PLATOON = 3,
	COMPANY = 4,
	BATTALION = 5,
	REGIMENT = 6, // aka BRIGADE
	DIVISION = 7,
	CORPS = 8,
	ARMY = 9,

	ROOT = 255
};

static ETPSUnitLevel _TPSLevel_Up(ETPSUnitLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl >= 7)
	{
		return ETPSUnitLevel::ROOT;
	}

	return static_cast<ETPSUnitLevel>(lvl + 1);
}

static ETPSUnitLevel _TPSLevel_Down(ETPSUnitLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl <= 0)
	{
		return ETPSUnitLevel::ROOT;
	}

	return static_cast<ETPSUnitLevel>(lvl - 1);
}

static FString _TPSHierarchicalLevelToString(ETPSUnitLevel level)
{
	switch (level)
	{
	case ETPSUnitLevel::UNIT:
		return "Unit";
	case ETPSUnitLevel::FIRE_TEAM:
		return "FireTeam";
	case ETPSUnitLevel::SQUAD:
		return "Squad";
	case ETPSUnitLevel::PLATOON:
		return "Platoon";
	case ETPSUnitLevel::COMPANY:
		return "Company";
	case ETPSUnitLevel::BATTALION:
		return "Battalion";
	case ETPSUnitLevel::REGIMENT:
		return "Regiment";
	case ETPSUnitLevel::DIVISION:
		return "Division";
	case ETPSUnitLevel::ARMY:
		return "Army";
	case ETPSUnitLevel::ROOT:
	default:
		return "[UNIT]";
	}
}


static FString _TPSHierarchicalLevelAbbreviationToString(ETPSUnitLevel level)
{
	switch (level)
	{
	case ETPSUnitLevel::UNIT:
		return "U";
	case ETPSUnitLevel::FIRE_TEAM:
		return "F";
	case ETPSUnitLevel::SQUAD:
		return "S";
	case ETPSUnitLevel::PLATOON:
		return "P";
	case ETPSUnitLevel::COMPANY:
		return "C";
	case ETPSUnitLevel::BATTALION:
		return "B";
	case ETPSUnitLevel::REGIMENT:
		return "R";
	case ETPSUnitLevel::DIVISION:
		return "D";
	case ETPSUnitLevel::ARMY:
		return "A";
	case ETPSUnitLevel::ROOT:
	default:
		return "X";
	}
}
