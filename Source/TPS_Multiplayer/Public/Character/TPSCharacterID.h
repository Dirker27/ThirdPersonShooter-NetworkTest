// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacterID.generated.h"


/**
 * Identifier to specify a specific Character.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSCharacterID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid Guid;


public:
	bool operator == (const FTPSCharacterID other) const
	{
		return Guid == other.Guid;
	}

	FString ToString() const
	{
		return Guid.ToString();
	}

	friend uint32 GetTypeHash(const FTPSCharacterID& id)
	{
		return uint32(CityHash64((char*)&id.Guid, sizeof(FGuid)));
	}
};


UCLASS(BlueprintType)
class UTPSCharacterIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSCharacterID CharacterID;
};