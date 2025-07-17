// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSHierarchicalCollectionLevel.generated.h"

UENUM(BlueprintType)
enum class ETPSHierarchicalLevel : uint8
{
	UNIT = 0,
	FIRE_TEAM = 1,
	SQUAD = 2,
	PLATOON = 3,
	COMPANY = 4,
	DIVISION = 5,
	BATTALION = 6,
	ARMY = 7,

	ROOT = 255
};

static ETPSHierarchicalLevel LevelUp(ETPSHierarchicalLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl >= 7)
	{
		return ETPSHierarchicalLevel::ROOT;
	}

	return static_cast<ETPSHierarchicalLevel>(lvl + 1);
}

static ETPSHierarchicalLevel LevelDown(ETPSHierarchicalLevel level)
{
	int lvl = static_cast<int>(level);
	if (lvl <= 0)
	{
		return ETPSHierarchicalLevel::ROOT;
	}

	return static_cast<ETPSHierarchicalLevel>(lvl - 1);
}
