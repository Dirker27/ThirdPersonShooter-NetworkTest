// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamDefinition.h"
#include "TPSTeamID.h"

#include "Army/TPSArmy.h"
#include "World/Spawn/TPSSpawnPool.h"

#include "TPSTeamInstance.generated.h"

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSTeamInstance();

    // Broadcast Delegate - Team/Character/Player Roster Updated
    UPROPERTY(BlueprintAssignable)
    FArmyUpdate ArmyUpdate;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;

//~ ==================================================================== ~//
//  ATTRIBUTES
//~ ==================================================================== ~//
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
    TEnumAsByte<ETPSTeamID> TeamID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSTeamConfig Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSSpawnPool> SpawnPool;

//~ ==================================================================== ~//
//  LIVE STATE
//~ ==================================================================== ~//
public:
    // TODO: Create a "scoreboard" record for more detailed attribution.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    int ScoredPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSArmyInstance>> Armies;

    // All *active* (connected) players that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> ActivePlayers;
};
