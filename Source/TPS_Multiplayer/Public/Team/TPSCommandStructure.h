// (C) ToasterCat Studios 2025

#pragma once

#include "Team/TPSCommandConfiguration.h"
#include "Team/TPSCommandGroup.h"

#include "TPSCommandStructure.generated.h"

// A Hierarchical group that can be issued commands that will chain down to
//   child units.
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandStructure : public UTPSCommandGroup
{
	GENERATED_BODY()

public:
	UTPSCommandStructure();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATPSCharacter> Leader;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTPSCommandConfiguration> Configuration;
};
