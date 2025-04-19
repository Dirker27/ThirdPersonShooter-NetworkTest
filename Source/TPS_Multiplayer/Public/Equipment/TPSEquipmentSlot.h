// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSEquipmentSlot.generated.h"

UENUM(BlueprintType)
enum ETPSEquipmentHarnessSlot : int
{
	PrimaryHand,
	SecondaryHand,

	Headgear_Cap,
	Headgear_Eyes,
	Backpack,
	Chestpiece,
	Belt,

	LegHolster_Left,
	LegHolster_Right,

	ChestHolster_Pistol,
	ChestHolster_Rifle,

	BackHolster_Left,
	BackHolster_Right,
	BackHolster_Cross,

	WeaponBelt_Front,
	WeaponBelt_Rear,
	WeaponBelt_Left,
	WeaponBelt_Right,
};

static const char* ETPSEquipmentHarnessSlotToString(const ETPSEquipmentHarnessSlot slot) {
	switch (slot) {
	case PrimaryHand:
		return "PrimaryWeaponHand";
	case SecondaryHand:
		return "SecondaryWeaponHand";

	case Headgear_Cap:
		return "Headgear_Cap";
	case Headgear_Eyes:
		return "Headgear_Eyes";
	case Backpack:
		return "Backpack";
	case Chestpiece:
		return "Chestpiece";
	case Belt:
		return "Belt";

	case LegHolster_Left:
		return "LegHolster-Left";
	case LegHolster_Right:
		return "LegHolster-Right";

	case ChestHolster_Pistol:
		return "PistolChestHolster";
	case ChestHolster_Rifle:
		return "RifleChestHolster";


	case BackHolster_Left:
		return "LeftBackHolster";
	case BackHolster_Right:
		return "RightBackHolster";
	case BackHolster_Cross:
		return "CrossBackHolster";

	case WeaponBelt_Front:
		return "BeltHolster-Front";
	case WeaponBelt_Rear:
		return "BeltHolster-Rear";
	case WeaponBelt_Left:
		return "BeltHolster-Left";
	case WeaponBelt_Right:
		return "BeltHolster-Right";

	default:
		return "UNSUPPORTED";
	}
}
