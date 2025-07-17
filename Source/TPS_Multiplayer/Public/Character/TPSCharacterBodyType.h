#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSCharacterBodyType.generated.h"

UENUM(BlueprintType)
enum class ETPSCharacterBodyType : uint8
{
    Male = 0,
    Female = 1
};

