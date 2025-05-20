// (C)ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSHitDirection.generated.h"

UENUM(BlueprintType)
enum EHitDirection : int
{
	Front = 0,
	Back = 1,
	Left = 2,
	Right = 3
};

UENUM(BlueprintType)
enum EHitForce : int
{
	Light = 0,
	Medium = 1,
	Heavy = 2
};