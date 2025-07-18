// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCommandUnitConfiguration.generated.h"

// Schema and rules for the current level of a command unit
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSCommandUnitConfiguration
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumSubCollections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumMembers;

	// TODO: Leader Inheritance Policy, Structure expand/coalesce rules, additional commands, etc
};