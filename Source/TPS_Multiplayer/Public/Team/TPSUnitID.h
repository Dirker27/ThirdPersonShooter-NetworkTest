// (C) ToasterCat Studios 2025

#pragma once

#include "Components/ActorComponent.h"

#include "Model/TPSHierarchicalCollectionLevel.h"
#include "Team/TPSTeamID.h"

#include "TPSUnitID.generated.h"


/*USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitNumber
{
	GENERATED_BODY()

	uint8 Number;
};
typedef uint8 FTPSUnitNumber;
*/

/**
 * Identifier to specify an exact unit in a given Team.
 *
 * Will be unique for each team, but may be duplicated across multiple teams.
 *   ie: there can be an 'Abel Squad' on the other Team as well as your own.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSUnitID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSTeamID TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSHierarchicalLevel UnitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 UnitNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETPSHierarchicalLevel, uint8> Hierarchy;
};



UCLASS(BlueprintType)
class UTPSUnitIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSUnitID ID;
};