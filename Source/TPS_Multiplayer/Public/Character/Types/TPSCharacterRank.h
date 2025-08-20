// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterRank.generated.h"

UENUM(BlueprintType)
enum class ETPSCharacterRank : uint8
{
    Recruit = 0,

    Private = 10,
    PrivateFirstClass = 11,
    PrivateSecondClass = 12,

    Corporal = 20,

    Sergeant       = 30,
    StaffSergeant  = 31,
    MasterSergeant = 33,
    FirstSergeant  = 34,
    SergeantMajor  = 39,

    Lieutenant = 40,
    FirstLieutenant = 41,
    SecondLieutenant = 42,

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


static FString TPSCharacterRankAbbreviationString(ETPSCharacterRank rank)
{
    switch (rank)
    {
    case ETPSCharacterRank::Recruit:
        return "Rct";
    case ETPSCharacterRank::Private:
    case ETPSCharacterRank::PrivateFirstClass:
    case ETPSCharacterRank::PrivateSecondClass:
        return "Pvt";
    case ETPSCharacterRank::Corporal:
        return "Cpl";
    case ETPSCharacterRank::Sergeant:
    case ETPSCharacterRank::StaffSergeant:
    case ETPSCharacterRank::MasterSergeant:
    case ETPSCharacterRank::FirstSergeant:
    case ETPSCharacterRank::SergeantMajor:
        return "Sgt";
    case ETPSCharacterRank::Lieutenant:
        return "Lt";
    case ETPSCharacterRank::Captain:
        return "Cpt";
    case ETPSCharacterRank::Major:
        return "Mjr";
    case ETPSCharacterRank::Colonel:
    case ETPSCharacterRank::LieutenantColonel:
        return "Col";
    case ETPSCharacterRank::General:
    case ETPSCharacterRank::BrigadierGeneral:
    case ETPSCharacterRank::LieutenantGeneral:
    case ETPSCharacterRank::MajorGeneral:
        return "Gen";
    case ETPSCharacterRank::CommanderInChief:
        return "Cmdr";
    default:
        return "[RANK]";
    }
}