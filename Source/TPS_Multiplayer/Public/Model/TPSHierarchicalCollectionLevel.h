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
