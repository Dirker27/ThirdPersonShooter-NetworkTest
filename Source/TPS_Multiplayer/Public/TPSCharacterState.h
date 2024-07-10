#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSCharacterState.generated.h"

UENUM(BlueprintType)
enum ETPSCharacterState : int
{
    Casual = 0,
    Combat = 1,
    Injured = 2,
    Incapacitated = 3,
    Mounted = 4
};

