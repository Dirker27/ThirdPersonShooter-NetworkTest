// (C) ToasterCat Studios 2025

#pragma once

#include "TPSUnitID.generated.h"

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
	FGuid Guid;

public:
	bool operator == (const FTPSUnitID other) const
	{
		return Guid == other.Guid;
	}

	friend uint32 GetTypeHash(const FTPSUnitID& id)
	{
		return uint32(CityHash64((char*)&id.Guid, sizeof(FGuid)));
	}
};


static FString TPSUnitIdToString(const FTPSUnitID unitId)
{
	return unitId.Guid.ToString();
}


UCLASS(BlueprintType)
class UTPSUnitIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTPSUnitID ID;
};
