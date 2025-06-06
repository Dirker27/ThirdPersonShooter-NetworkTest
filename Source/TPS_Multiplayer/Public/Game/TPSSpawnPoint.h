// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Team/TPSSquad.h"
#include "Team/TPSTeamID.h"

#include "TPSSpawnPoint.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSSPawnPoint : public AActor //: public APlayerStart
{
    GENERATED_BODY()

public:
    ATPSSPawnPoint();

    // Teams that can use this Spawn Point.
    //   If unset, then all can use.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETPSTeamID> TeamWhitelist;
};
