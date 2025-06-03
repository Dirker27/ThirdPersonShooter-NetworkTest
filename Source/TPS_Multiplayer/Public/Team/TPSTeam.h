// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSPlatoon.h"
#include "TPSSquad.h"
#include "TPSTeamConfiguration.h"
#include "TPSTeamID.h"

#include "TPSTeam.generated.h"

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeam : public UObject
{
    GENERATED_BODY()

public:
    UTPSTeam();

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSTeamID ID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSTeamConfiguration Configuration;

    ////////////////////////////////////////////////////////
    // State

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSPlatoonID> ActivePlatoons;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSSquadID> ActiveSquads;
};
