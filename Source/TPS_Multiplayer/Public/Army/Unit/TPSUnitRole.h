// (C) ToasterCat Studios 2025

#pragma once

#include "TPSUnitRole.generated.h"

// A Character's "Class" by a different name.
UENUM(BlueprintType)
enum ETPSUnitRole : int
{
    Rifleman = 0,
    AutomaticRifleman = 1,
    Grenadier = 2,
    Sharpshooter = 3,

    Leader = 10,
    AssistantLeader = 11,

    // Radio, EW Operator, Engineer, etc
    Specialist,
    Medic,

    // MG, Sniper, Mortar, etc
    Gunner,
    AssistantGunner
};
