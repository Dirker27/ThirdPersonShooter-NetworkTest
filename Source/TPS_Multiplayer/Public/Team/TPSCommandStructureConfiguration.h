// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCommandStructureConfiguration.generated.h"

// Schema and rules for the current level of a command unit
UCLASS()
class TPS_MULTIPLAYER_API UTPSCommandStructureConfiguration : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumSubCollections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumMembers;

	// TODO: Leader Inheritance Policy, Structure expand/coalesce rules, additional commands, etc
};