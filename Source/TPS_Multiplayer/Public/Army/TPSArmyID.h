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
	FGuid ID;
};


UCLASS(BlueprintType)
class UTPSArmyIdentifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTPSArmyID ID;
};