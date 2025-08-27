// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterRecord.generated.h"


USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSOperatorRecord
{
    GENERATED_BODY()

    // XP Points
    UMETA("XP")
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ExperiencePoints;

    // Lifetime Kill Count
    UMETA("Kills")
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int KillCount;

    // Lifetime Mission Count
    UMETA("Deployments")
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Deploymentcount;

    // TODO: Store events, interactions, stats (Alpha Demo)
};


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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSOperatorRecord Record;
};
