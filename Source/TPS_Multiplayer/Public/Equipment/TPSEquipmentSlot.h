// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TPSEquipmentSlot.generated.h"

UENUM(BlueprintType)
enum ETPSEquipmentSlot : int
{
	None,
	PrimaryWeapon,
	SecondaryWeapon,
	TertiaryWeapon,
	LethalEquipment,
	TacticalEquipment,

	Helmet,
	PlateCarrier,
	Pack
};

static const char* ETPSEquipmentSlotToString(const ETPSEquipmentSlot slot) {
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


UENUM(BlueprintType)
enum ETPSEquipmentHarnessSlot : int
{
	PrimaryHand,
	SecondaryHand,

	Headgear,
	Backpack,
	Vest,
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

	case Headgear:
		return "HeadGear";
	case Backpack:
		return "Backpack";
	case Vest:
		return "Vest";
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
