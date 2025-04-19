// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSWeaponSlot.generated.h"

// TODO: Rename to WeaponSlot(?)
UENUM(BlueprintType)
enum ETPSWeaponSlot : int
{
	None = 0,
	PrimaryWeapon = 1,
	SecondaryWeapon = 2,
	TertiaryWeapon = 3,
	LethalEquipment = 4,
	TacticalEquipment = 5
};

static const char* ETPSWeaponSlotToString(const ETPSWeaponSlot slot) {
	switch (slot) {
	case None:
		return "None";
	case PrimaryWeapon:
		return "PrimaryWeapon";
	case SecondaryWeapon:
		return "SecondaryWeapon";
	case TertiaryWeapon:
		return "TertiaryWeapon";
	case LethalEquipment:
		return "LethalEquipment";
	case TacticalEquipment:
		return "TacticalEquipment";
	default:
		return "UNSUPPORTED";
	}
}
