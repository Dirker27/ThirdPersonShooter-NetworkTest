// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Team/TPSTeamID.h"
#include "Team/TPSUnitID.h"

#include "TPSSpawnPoint.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSSPawnPoint : public AActor //: public APlayerStart
{
    GENERATED_BODY()

public:
    ATPSSPawnPoint();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int ID;

    // Teams that can use this Spawn Point.
    //   If unset, then all can use.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETPSTeamID TeamID;

    // Squads in a Team that can use this Spawn Point
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FUnitID AssignedUnit;



    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UBoxComponent> Collider;
};
