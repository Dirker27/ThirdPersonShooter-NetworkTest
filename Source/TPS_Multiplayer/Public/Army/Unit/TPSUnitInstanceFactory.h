// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCommandUnit.h"

#include "TPSUnitInstanceFactory.generated.h"


// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _SquadNameList = {
    "Wrecker",
    "Zombie",
    "Warlock",
    "Ghost",
    "Wraith",
    "Rogue",
    "Cypher",
    "Pixie",
    "Gypsy",
    "Revenant",
    "Gamma",
    "Hulk",
    "Iron",
    "Steel",
    "Dagger",
    "Sword",
    "Joker",
    "Shepard",
    "Wilco",
    "Panther",
    "Tiger",
    "Leeroy",
    "Spartan",
    "Wraith",
    "Abel",
    "Whiskey",
    "Joker",
    "Harvard",
    "Ivy",
    "Yale",
    "Terra",
    "Stonewall",
    "Delta",
    "Scooby",
    "Panda",
    "Grizzly",
    "Tiger",
    "Libra",
    "Cancer",
    "Gamma"
};



/**
 * Factory class to create and configure Armies and their SubUnits
 *
 * TODO: https://toastercatstudios.atlassian.net/browse/PC-238
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSUnitInstanceFactory : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSUnitInstanceFactory();

    //~ ==================================================================== ~//
    //  OPERATIONS
    //~ ==================================================================== ~//
public:
    UTPSCommandUnit* NewUnit() { /* STUB */ return nullptr; }

    static FString RandomSquadName()
    {
        int max = _SquadNameList.Num() - 1;
        int i = FMath::RandRange(0, max);
        return _SquadNameList[i];
    }
};

