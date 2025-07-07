// (C) ToasterCat Studios 2025

#pragma once
#include "TPSUnitID.h"
#include "Model/TPSHierarchicalCollection.h"

#include "TPSCommandGroup.generated.h"

// Base collection of units that can be given a command.
//
// IGNORES hierarchical behavior to allow for flat-hierarchy groups.
//   ie: Specific Squads and Platoons and Units that may not share hierarchy (selection groups)
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandGroup : public UTPSHierarchicalCollection
{
	GENERATED_BODY()

public:
	UTPSCommandGroup();

	// Unique ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnitID ID;

	void Command();
};
