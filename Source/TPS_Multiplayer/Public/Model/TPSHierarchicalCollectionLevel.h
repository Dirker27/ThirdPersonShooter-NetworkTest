// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSHierarchicalCollectionLevel.generated.h"

UENUM(BlueprintType)
enum ETPSHierarchicalLevel : int
{
	ANY = 0,

	UNIT = 1,
	TEAM = 2,
	SQUAD = 3,
	PLATOON = 4,
	COMPANY = 5,
	DIVISION = 6,
	BATTALION = 7,

	SUPREME = INT_MAX
};
