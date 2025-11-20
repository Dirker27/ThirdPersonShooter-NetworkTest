// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSArmyID.h"
#include "TPSArmyIdentity.h"
#include "Engine/DataAsset.h"
#include "Unit/TPSUnitSchema.h"

#include "TPSArmyDefinition.generated.h"

/**
 * Detailed configuration options for an operator / character.
 */
USTRUCT(BlueprintType)
struct TPS_MULTIPLAYER_API FTPSArmyDefinitionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSUnitSchema> RootUnitSchema;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSArmyIdentity> Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSArmyID ArmyID;
};

UCLASS()
class TPS_MULTIPLAYER_API UTPSArmyDefinition : public UDataAsset
{
    GENERATED_BODY()

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSArmyDefinitionData Definition;
};