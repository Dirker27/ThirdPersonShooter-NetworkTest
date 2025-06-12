// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterRank.generated.h"

UENUM(BlueprintType)
enum class ETPSCharacterRank : uint8
{
    Recruit = 0,

    Private = 10,

    Corporal = 20,

    Sergeant       = 30,
    StaffSergeant  = 31,
    MasterSergeant = 33,
    FirstSergeant  = 34,
    SergeantMajor  = 39,

    Lieutenant = 40,
    Captain = 49,

    Major = 50,

    LieutenantColonel = 68,
    Colonel           = 69,

    BrigadierGeneral  = 71,
    MajorGeneral      = 72,
    LieutenantGeneral = 73,
    General           = 79,

    CommanderInChief = 99
};
