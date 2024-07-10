// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSLocomotionState.generated.h"

UENUM(BlueprintType)
enum ETPSLocomotionState : int
{
    Standing = 0,
    Crouching = 1,
    Prone = 2,
    Sprinting = 3,
    Ragdoll = 4
};

