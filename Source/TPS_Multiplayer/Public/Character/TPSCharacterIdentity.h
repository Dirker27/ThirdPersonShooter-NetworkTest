// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterBodyType.h"
#include "TPSCharacterRank.h"
#include "Team/TPSFactionID.h"
#include "Team/TPSTeamID.h"

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
    TEnumAsByte<ETPSCharacterRank> Rank;

    // Team Alignment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TEnumAsByte<ETPSTeamID> TeamID;

    // Faction Alignment
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TEnumAsByte<ETPSFactionID> FactionID;

    // Assigned Squad/Unit
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int CommandGroupID;


};
