// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "TPSCharacterLocomotionState.generated.h"

UENUM(BlueprintType)
enum ETPSCharacterLocomotionState : int
{
    Standing  = 0 UMETA(DisplayName = "Standing"),
    Crouching = 1 UMETA(DisplayName = "Crouching"),
    Prone     = 2 UMETA(DisplayName = "Prone"),
    Sprinting = 3 UMETA(DisplayName = "Sprinting"),
    Ragdoll   = 4 UMETA(DisplayName = "Ragdoll")
};


static std::unordered_map<char*, ETPSCharacterLocomotionState> ETPSLocomotionStateMap {
    { "Standing", Standing },
    { "Crouching", Crouching },
    { "Prone", Prone },
    { "Sprinting", Sprinting },
    { "Ragdoll", Ragdoll }
};

static const ETPSCharacterLocomotionState ETPSLocomotionStateFromString(char* state) {
    return ETPSLocomotionStateMap[state];
}

static const char* ETPSLocomotionStateToString(const ETPSCharacterLocomotionState state) {
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