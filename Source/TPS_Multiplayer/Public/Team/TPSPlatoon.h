#pragma once

#include "Team/TPSSquadID.h"

#include "TPSPlatoon.generated.h"

UENUM(BlueprintType)
enum class ETPSPlatoonID : uint8
{
    Able = 0,
    Bravo = 1
};


UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSPlatoon : public UObject
{
    GENERATED_BODY()

public:
    UTPSPlatoon();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSPlatoonID ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSSquadID> ActiveSquads;
};
