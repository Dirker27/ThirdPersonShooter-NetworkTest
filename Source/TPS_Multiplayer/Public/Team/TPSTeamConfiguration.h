// (C) ToasterCat Studios 2025

#pragma once

#include "TPSTeamConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FTPSTeamConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETPSHierarchicalLevel> HighestSupportedCommandLevel = ETPSHierarchicalLevel::PLATOON;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETPSHierarchicalLevel>, int> LevelCapacityMap;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumPlatoonsPerCompany = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumSquadsPerPlatoon = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumFireteamsPerSquad = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxNumUnitsPerFireteam = 4;*/
};




UCLASS()
class UTPSTeamConfigurationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSTeamConfiguration Configuration;
};