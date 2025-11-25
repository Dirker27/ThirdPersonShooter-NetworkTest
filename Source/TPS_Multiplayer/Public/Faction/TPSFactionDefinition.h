// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TPSFactionIdentity.h"
#include "TPSFactionID.h"

#include "TPSFactionDefinition.generated.h"

/**
 * Detailed configuration options for an operator / character.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSFactionDefinitionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSFactionID> FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSFactionIdentity> Identity;
};

UCLASS()
class TPS_MULTIPLAYER_API UTPSFactionDefinition : public UDataAsset
{
    GENERATED_BODY()

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSFactionDefinitionData Definition;
};