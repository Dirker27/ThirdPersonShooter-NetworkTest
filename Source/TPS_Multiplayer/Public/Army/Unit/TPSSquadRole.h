// (C) ToasterCat Studios 2025

#pragma once

#include "TPSSquadRole.generated.h"

// A Character's "Class" by a different name.
UENUM(BlueprintType)
enum ETPSSquadRole : int
{
    NONE = 0,

    Leader = 1,

    Rifleman,
    AutomaticRifleman,
    Support,
    Medic,

    // Radio, EW Operator, Engineer, etc
    Specialist,

    // MG, Sniper, Mortar, etc
    Gunner,
    // Loader, Spotter, etc
    AssistantGunner
};
