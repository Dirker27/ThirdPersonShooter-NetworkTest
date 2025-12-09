// (C) ToasterCat Studios 2025

#pragma once

#include "TPSPlayerID.generated.h"

/**
 * Identifier to specify a specific Player (cross-platform GUID).
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSPlayerID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid Guid;

	// Copied from PlayerState.PlayerId
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UEPlayerID;


public:
	bool operator == (const FTPSPlayerID other) const
	{
		return Guid == other.Guid;
	}

	FString ToString() const
	{
		return Guid.ToString();
	}

	friend uint32 GetTypeHash(const FTPSPlayerID& id)
	{
		return uint32(CityHash64((char*)&id.Guid, sizeof(FGuid)));
	}
};


UCLASS(BlueprintType)
class UTPSPlayerIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSPlayerID PlayerID;
};
