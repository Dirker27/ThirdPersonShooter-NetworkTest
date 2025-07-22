// (C) ToasterCat Studios 2025

#pragma once

#include "TPSTeamConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FTPSTeamConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSHierarchicalLevel HighestSupportedCommandLevel = ETPSHierarchicalLevel::PLATOON;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> MemberCapacityMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, int> SubUnitCapacityMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSSquadRole, UTPSEquipmentLoadout*> MemberLoadoutMap;
};




UCLASS(BlueprintType)
class UTPSTeamConfigurationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSTeamConfiguration Configuration;
};