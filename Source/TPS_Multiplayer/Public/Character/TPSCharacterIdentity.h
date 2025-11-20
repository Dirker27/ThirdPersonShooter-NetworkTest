// (C) ToasterCat Studios 2025

#pragma once

#include "Army/Unit/TPSUnitRole.h"
#include "Character/Types/TPSCharacterBodyType.h"
#include "Character/Types/TPSCharacterRank.h"
#include "Faction/TPSFactionID.h"
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
    TEnumAsByte<ETPSCharacterStance> PreferredStance;

    // Current Rank - granted and recognized independently of XP
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TEnumAsByte<ETPSCharacterRank> Rank;

    // Faction Alignment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TEnumAsByte<ETPSFactionID> FactionID;

    // Character's "Class" / Role in their Squad
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSUnitRole> SquadRole;
};

/**
 * ArmyID Card Pointer
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
