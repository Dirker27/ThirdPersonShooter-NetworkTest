// (C) ToasterCat Studios 2025

#pragma once

#include "Army/Unit/TPSHierarchicalCollection.h"

#include "TPSCommandGroup.generated.h"

// Base collection of units that can be given a command.
//
// IGNORES hierarchical behavior to allow for flat-hierarchy groups.
//   ie: Specific Squads and Platoons and Units that may not share hierarchy
//   (selection groups, "Task Force"s, etc)
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandGroup : public UTPSHierarchicalCollection
{
	GENERATED_BODY()

public:
	UTPSCommandGroup();

	// Display Name for the Unit
	//   ('Easy' Company, 'Abel' Squad, etc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Alias;


	UFUNCTION(BlueprintCallable)
	void Command();
};
