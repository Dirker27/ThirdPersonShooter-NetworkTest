// (C) ToasterCat Studios 2025

#pragma once

#include "Character/Types/TPSCharacterBodyType.h"
#include "Character/Types/TPSCharacterRank.h"
#include "Team/Types/TPSFactionID.h"
#include "Team/Types/TPSSquadRole.h"
#include "Team/Types/TPSUnitID.h"
#include "Types/TPSCharacterStance.h"

#include "TPSCharacterIdentity.generated.h"


/**
 * A Character's "UnitID Card"
 *
 * If we were to serialize this Character as a PERSON, what would we see?
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSOperatorIdentity
{
    GENERATED_BODY()

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:

    // Unique ID - independent of UnitID.
    //   Used for lookup and management operations from GameMode / GameState.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGuid Guid;



    ////////////////////////////////////////////////////////
    // Name

    // Character's First Name - "Steve", "Bob", "Jenny", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString FirstName;

    // Character's Last Name - "Williams", "Stevenson", "O'Rourke", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString LastName;

    // Character's Callsign - "Iceman", "Hard Tack", "Thrill", etc.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString Callsign;

    //UFUNCTION(BlueprintCallable)
    FString FullName() const
    {
        return Callsign.IsEmpty()
            ? FString::Printf(TEXT("%s %s"),
                *FirstName,
                *LastName)
            : FString::Printf(TEXT("%s '%s' %s"),
                *FirstName,
                *Callsign,
                *LastName);
    }

    ////////////////////////////////////////////////////////
    // Details and Alignment


    // Masculine or Feminine ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSCharacterBodyType CharacterBodyType;

    // Left or Right-Handed ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSCharacterStance PreferredStance;

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
    TEnumAsByte<ETPSSquadRole> SquadRole;
};

/**
 * ID Card Pointer
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterIdentity : public UObject
{
    GENERATED_BODY()

public:
    UTPSCharacterIdentity();

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

    //~ ==================================================================== ~//
    //  ATTRIBUTES
    //~ ==================================================================== ~//
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSOperatorIdentity OpID;
};
