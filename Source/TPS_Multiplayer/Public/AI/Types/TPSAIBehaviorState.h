// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSAIBehaviorState.generated.h"

/**
 * The state/mode of a given character's AI behavior tree.
 *   (what sub-tree to execute in Selectors)
 */
UENUM(BlueprintType)
enum AIBehaviorState : int
{
	Dormant,
	Patrol,
	Engaging
};
