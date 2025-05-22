// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSSquadID.h"

#include "TPSSquad.generated.h"

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSSquad : public UObject
{
    GENERATED_BODY()

public:
    UTPSSquad();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ETPSSquadID ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSSquadID> ActiveSquads;
};
