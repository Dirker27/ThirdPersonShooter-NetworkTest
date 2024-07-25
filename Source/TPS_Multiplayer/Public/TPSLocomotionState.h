// (C) ToasterCat Studios 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSLocomotionState.generated.h"

UENUM(BlueprintType)
enum ETPSLocomotionState : int
{
    Standing = 0,
    Crouching = 1,
    Prone = 2,
    Sprinting = 3,
    Ragdoll = 4
};


static std::unordered_map<char*, ETPSLocomotionState> ETPSLocomotionStateMap {
    { "Standing", Standing },
    { "Crouching", Crouching },
    { "Prone", Prone },
    { "Sprinting", Sprinting },
    { "Ragdoll", Ragdoll }
};

static const ETPSLocomotionState ETPSLocomotionStateFromString(char* state) {
    return ETPSLocomotionStateMap[state];
}

static const char* ETPSLocomotionStateToString(const ETPSLocomotionState state) {
	switch (state) {
	case Standing:
		return "Standing";
	case Crouching:
		return "Crouching";
	case Prone:
		return "Prone";
	case Sprinting:
		return "Sprinting";
	case Ragdoll:
		return "Ragdoll";
	default:
		return "UNSUPPORTED";
	}
}