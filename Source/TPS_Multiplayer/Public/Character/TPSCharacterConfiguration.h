// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Character/Types/TPSCharacterBodyType.h"

#include "TPSCharacterConfiguration.generated.h"


UCLASS()
class TPS_MULTIPLAYER_API UTPSCharacterConfiguration : public UDataAsset
{
    GENERATED_BODY()

protected:
    virtual bool IsSupportedForNetworking() const override { return true; }

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSCharacterBodyType BodyType;

    // How accurate the Character's shooting arm is in DEGREES of freedom.
    //    TODO: Move to "CharacterAttributes" object
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseAccuracyTolerance = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IdleThresholdSeconds = 3.f;
};