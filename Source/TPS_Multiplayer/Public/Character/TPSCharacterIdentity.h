// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterBodyType.h"
#include "TPSCharacterRank.h"
#include "Team/TPSFactionID.h"
#include "Team/TPSSquadRole.h"
#include "Team/TPSUnitID.h"

#include "TPSCharacterIdentity.generated.h"


/**
 * A Character's "ID Card"
 *
 * If we were to serialize this Character as a PERSON, what would we see?
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterIdentity : public UObject
{
    GENERATED_BODY()

public:
    UTPSCharacterIdentity();

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:

    ////////////////////////////////////////////////////////
    // Identity

    // Character's Name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName Name;

    // Unique ID
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString UUID;

    // Masculine / Feminine ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSCharacterBodyType> CharacterBodyType;

    // Current Rank - granted and recognized independently of XP
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSCharacterRank Rank;

    // Faction Alignment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSFactionID FactionID;

    // Unit ID -> Squad/Team Hierarchy
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSUnitID UnitID;

    // Character's "Class" / Role in their Squad
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSSquadRole SquadRole;
};
