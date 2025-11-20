// (C) ToasterCat Studios 2025

#pragma once

#include "Army/TPSArmyDefinition.h"

#include "TPSTeamDefinition.generated.h"


/**
 * Describes the behaviors of a team.
 */
USTRUCT(BlueprintType)
struct FTPSTeamConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowFriendlyFire = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowCrossArmySpawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowCrossArmyPossession = true;
};


/**
 * Describes the makeup of a team to be instantiated.
 */
USTRUCT(BlueprintType)
struct FTPSTeamDefinitionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UTPSArmyDefinition>> Armies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSTeamConfig Configuration;
};




UCLASS(BlueprintType)
class UTPSTeamDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSTeamDefinitionData Definition;
};
