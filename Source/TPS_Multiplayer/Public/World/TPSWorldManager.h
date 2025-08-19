// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Game/TPSGameState.h"
#include "Team/TPSTeamInstance.h"

#include "TPSWorldManager.generated.h"


/**
 * Responsible for spawning and tracking loose objects in the world.
 *   (equipment, dead characters, world objects, etc)
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSWorldManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSWorldManager();
};
