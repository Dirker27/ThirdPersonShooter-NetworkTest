// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSSquad.h"
#include "TPSTeamID.h"

#include "TPSTeam.generated.h"

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeam : public UObject
{
    GENERATED_BODY()

public:
    UTPSTeam();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSTeamID ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSSquadID> ActiveSquads;
};
