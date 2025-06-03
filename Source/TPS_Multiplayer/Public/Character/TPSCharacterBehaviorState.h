#pragma once

//#include "string.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSCharacterBehaviorState.generated.h"

UENUM(BlueprintType)
enum ETPSCharacterBehaviorState : int
{
    Casual = 0,
    Combat = 1,
    Injured = 2,
    Incapacitated = 3,
    Mounted = 4
};

static std::unordered_map<char*, ETPSCharacterBehaviorState> ETPSCharacterStateMap {
    { "Casual", Casual },
    { "Combat", Combat },
    { "Injured", Injured },
    { "Incapacitated", Incapacitated },
    { "Mounted", Mounted }
};

static const ETPSCharacterBehaviorState ETPSCharacterStateFromString(char* state) {
    return ETPSCharacterStateMap[state];
}

static const char* ETPSCharacterStateToString(const ETPSCharacterBehaviorState state) {
    switch (state) {
    case Casual:
        return "Casual";
    case Combat:
        return "Combat";
    case Injured:
        return "Injured";
    case Incapacitated:
        return "Incapacitated";
    default:
        return "UNSUPPORTED";
    }
}