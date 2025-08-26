// (C) ToasterCat Studios 2025

#include "Character/TPSCharacterIdentity.h"

UTPSCharacterIdentity::UTPSCharacterIdentity()
{
    Guid = FGuid::NewGuid();
}

FString UTPSCharacterIdentity::GetFormalDisplayName() const
{
    FString displayName = TPSCharacterRankAbbreviationString(Rank) + ". ";
    displayName += FirstName + " ";
    if (!Callsign.IsEmpty())
    {
        displayName += "'" + Callsign + "'";
    }
    displayName += LastName;

    return displayName;
}