// (C) ToasterCat Studios 2025

#pragma once

#include "TPSArmyID.generated.h"


/**
 * Identifier to specify a specific Army.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSArmyID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid Guid;


public:
	bool operator == (const FTPSArmyID other) const
	{
		return Guid == other.Guid;
	}

	FString ToString() const
	{
		return Guid.ToString();
	}

	friend uint32 GetTypeHash(const FTPSArmyID& id)
	{
		return uint32(CityHash64((char*)&id.Guid, sizeof(FGuid)));
	}
};


UCLASS(BlueprintType)
class UTPSArmyIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSArmyID ArmyID;
};