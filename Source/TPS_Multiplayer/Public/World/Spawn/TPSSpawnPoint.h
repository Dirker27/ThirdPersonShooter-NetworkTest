// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "Army/Unit/TPSCommandUnit.h"
#include "Army/Unit/TPSUnitHierarchy.h"
#include "Components/BoxComponent.h"

#include "Army/Unit/TPSUnitRole.h"
#include "Team/TPSTeamID.h"

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<ETPSTeamID> AssignedTeamID;

    // Filters Squads/Fireteams that can use this Spawn Point
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTPSUnitHierarchy UnitHierarchyFilter;

    // Will override all other selection filters if set.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<UTPSCommandUnit> AssignedUnit;

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
