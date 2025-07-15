// (C) ToasterCat Studios 2025

#pragma once

#include "Game/TPSSpawnPool.h"
#include "Team/TPSCommandStructureConfiguration.h"
#include "Team/TPSCommandGroup.h"

#include "TPSCommandStructure.generated.h"

// A Hierarchical group that can be issued commands that will chain down to
//   child units.
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandStructure : public UTPSCommandGroup
{
	GENERATED_BODY()

protected:
	UTPSCommandStructure();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSCharacterInstance> Leader;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSCommandStructureConfiguration> Configuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSSpawnPool> SpawnPool;
};
