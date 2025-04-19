// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSGearSlot.generated.h"

static const int GEAR_SLOT_SIZE = 4;
UENUM(BlueprintType)
enum ETPSGearSlot : int
{
	Headgear = 0,
	Eyewear = 1,
	Vest = 2,
	Pack = 3
};

static const char* ETPSGearSlotToString(const ETPSGearSlot slot) {
	switch (slot) {
	case Headgear:
		return "Helmet";
	case Eyewear:
		return "Helmet";
	case Vest:
		return "Vest";
	case Pack:
		return "Pack";
	default:
		return "UNSUPPORTED";
	}
}

