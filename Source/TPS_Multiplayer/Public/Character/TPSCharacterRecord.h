// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterRecord.generated.h"

/**
 * A Character's recorded stats, history, and experience.
 *
 * What will be persisted as this Character's progress between deployments.
 **/
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterRecord : public UObject
{
    GENERATED_BODY()

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:
    // XP Points
    UMETA("XP")
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int ExperiencePoints;

    // TODO: Store events, interactions, stats (Alpha Demo)
};
