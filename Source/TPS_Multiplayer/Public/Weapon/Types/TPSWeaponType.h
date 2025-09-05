// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSWeaponType.generated.h"

UENUM(BlueprintType)
enum ETPSWeaponType : int
{
    OpenHand = 0,
    Pistol = 1,
    Rifle = 2,
    Launcher = 3,
    Throwable = 4,
    Placeable = 5
};

static const char* ETPSWeaponTypeToString(const ETPSWeaponType type) {
	switch (type) {
	case OpenHand:
		return "OpenHand";
	case Pistol:
		return "Pistol";
	case Rifle:
		return "Rifle";
	case Launcher:
		return "Launcher";
	case Throwable:
		return "Throwable";
	case Placeable:
		return "Placeable";
	default:
		return "UNSUPPORTED";
	}
}