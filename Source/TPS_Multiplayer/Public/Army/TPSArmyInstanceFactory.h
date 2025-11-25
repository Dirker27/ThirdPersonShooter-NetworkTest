// (C) ToasterCat Studios 2025

#pragma once

#include "TPSArmy.h"

#include "TPSArmyInstanceFactory.generated.h"


/**
 * Factory class to create and configure Armies and their SubUnits
 *
 * TODO: https://toastercatstudios.atlassian.net/browse/PC-238
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSArmyInstanceFactory : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSArmyInstanceFactory();

    //~ ==================================================================== ~//
    //  OPERATIONS
    //~ ==================================================================== ~//
public:
    UTPSArmyInstance* NewArmyInstance() { /* STUB */ return nullptr; }
};

