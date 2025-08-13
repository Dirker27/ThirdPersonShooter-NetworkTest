// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterBodyType.h"
#include "TPSCharacterRank.h"
#include "Team/TPSFactionID.h"
#include "Team/TPSSquadRole.h"
#include "Team/TPSUnitID.h"

#include "TPSCharacterIdentity.generated.h"


/**
 * A Character's "UnitID Card"
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

    // Character's First Name - "Steve", "Bob", "Jenny", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName FirstName;

    // Character's Last Name - "Williams", "Stevenson", "O'Rourke", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName LastName;

    // Character's Callsign - "Iceman", "Hard Tack", "Thrill", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName Callsign;

    // Unique ID - independent of UnitID
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGuid Guid;

    // Masculine / Feminine ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSCharacterBodyType CharacterBodyType;

    // Current Rank - granted and recognized independently of XP
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSCharacterRank Rank;

    // Faction Alignment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSFactionID FactionID;

    // Unit UnitID -> Squad/Team Hierarchy
    //   Can be zero-d out if no assignment is active.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSUnitID UnitID;

    // Character's "Class" / Role in their Squad
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSSquadRole SquadRole;
};
