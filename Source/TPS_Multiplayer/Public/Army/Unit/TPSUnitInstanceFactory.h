// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCommandUnit.h"

#include "TPSUnitInstanceFactory.generated.h"


// TODO: [PC-228] Replace with JSON list and provide via Blueprint
static const TArray<FString> _SquadNameList = {

    // TODO: Migrate to NATO Constant
	"Alpha",
    "Bravo",
    "Charlie",
    "Delta",
    "Echo",
    "Foxtrot",
    "Golf",
    "Hotel",
    "India",
    "Juliett",
    "Kilo",
    "Lima",
    "Mike",
    "November"
    "Omega",
    "Papa",
    "Quebec",
    "Romeo",
    "Sierra",
    "Tango",
    "Uniform",
    "Victor",
    "Whiskey",
    "X-Ray",
    "Yankee",
    "Zulu",



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
    "Spartan"
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

