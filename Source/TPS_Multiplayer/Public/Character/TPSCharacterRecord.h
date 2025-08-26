// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterRecord.generated.h"

/**
 * A Character's recorded stats, history, and experience.
 *
 * What will be persisted as this Character's progress between deployments.
 *
 * TODO: Consider "CharacterHistory" ('Record' for DB persistence)
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterRecord : public UObject
{
    GENERATED_BODY()

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

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
