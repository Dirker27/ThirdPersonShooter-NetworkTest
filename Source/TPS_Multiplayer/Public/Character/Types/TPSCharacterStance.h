// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSCharacterStance.generated.h"

/**
 * The Character's current Stance - are they shooting right or left-handed?
 */
UENUM(BlueprintType)
enum ETPSCharacterStance : int
{
	RightHanded,
	LeftHanded
};