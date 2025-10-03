// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSHierarchicalCollectionLevel.generated.h"

UENUM(BlueprintType)
enum ETPSHierarchicalLevel : int
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

static ETPSHierarchicalLevel _TPSLevel_Up(ETPSHierarchicalLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl >= 7)
	{
		return ETPSHierarchicalLevel::ROOT;
	}

	return static_cast<ETPSHierarchicalLevel>(lvl + 1);
}

static ETPSHierarchicalLevel _TPSLevel_Down(ETPSHierarchicalLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl <= 0)
	{
		return ETPSHierarchicalLevel::ROOT;
	}

	return static_cast<ETPSHierarchicalLevel>(lvl - 1);
}

static FString _TPSHierarchicalLevelToString(ETPSHierarchicalLevel level)
{
	switch (level)
	{
	case ETPSHierarchicalLevel::UNIT:
		return "Unit";
	case ETPSHierarchicalLevel::FIRE_TEAM:
		return "FireTeam";
	case ETPSHierarchicalLevel::SQUAD:
		return "Squad";
	case ETPSHierarchicalLevel::PLATOON:
		return "Platoon";
	case ETPSHierarchicalLevel::COMPANY:
		return "Company";
	case ETPSHierarchicalLevel::BATTALION:
		return "Battalion";
	case ETPSHierarchicalLevel::REGIMENT:
		return "Regiment";
	case ETPSHierarchicalLevel::DIVISION:
		return "Division";
	case ETPSHierarchicalLevel::ARMY:
		return "Army";
	case ETPSHierarchicalLevel::ROOT:
	default:
		return "-UNIT-";
	}
}


static FString _TPSHierarchicalLevelAbbreviationToString(ETPSHierarchicalLevel level)
{
	switch (level)
	{
	case ETPSHierarchicalLevel::UNIT:
		return "U";
	case ETPSHierarchicalLevel::FIRE_TEAM:
		return "F";
	case ETPSHierarchicalLevel::SQUAD:
		return "S";
	case ETPSHierarchicalLevel::PLATOON:
		return "P";
	case ETPSHierarchicalLevel::COMPANY:
		return "C";
	case ETPSHierarchicalLevel::BATTALION:
		return "B";
	case ETPSHierarchicalLevel::REGIMENT:
		return "R";
	case ETPSHierarchicalLevel::DIVISION:
		return "D";
	case ETPSHierarchicalLevel::ARMY:
		return "A";
	case ETPSHierarchicalLevel::ROOT:
	default:
		return "X";
	}
}
