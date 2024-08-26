// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSWeaponType.generated.h"

UENUM(BlueprintType)
enum ETPSWeaponType : int
{
    Unarmed = 0,
    Pistol = 1,
    Rifle = 2,
    Launcher = 3,
    Throwable = 4,
    Placeable = 5
};
