// (C) ToasterCat Studios 2025

#pragma once

#include "TPSTeamConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FTPSTeamConfiguration
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETPSHierarchicalLevel> HighestSupportedCommandLevel = PLATOON;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumPlatoonsPerCompany = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumSquadsPerPlatoon = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumFireteamsPerSquad = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumMembersPerFireteam = 4;
};
