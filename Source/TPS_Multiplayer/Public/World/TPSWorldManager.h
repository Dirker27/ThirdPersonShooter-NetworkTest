// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Team/TPSTeam.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<ATPSEquipableItem>> LooseItems;
};
