// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Army/Unit/TPSUnitRole.h"
#include "Army/Unit/TPSUnitID.h"

#include "TPSSpawnPoint.generated.h"

UCLASS()
class TPS_MULTIPLAYER_API ATPSSpawnPoint : public AActor //public APlayerStart
{
    GENERATED_BODY()

public:
    ATPSSpawnPoint(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UBoxComponent> Collider;

public:
    // Squads in a Team that can use this Spawn Point
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSUnitID AssignedUnit;

    // Preferred Squad Role to spawn at this 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSUnitRole> PreferredSquadRoleToSpawn;

public:
    UFUNCTION(BlueprintCallable)
    void SetColliderColor(FColor color);

    UFUNCTION(BlueprintCallable)
    int GenerateUniqueSpawnId();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsSpawnAvailable();
};
