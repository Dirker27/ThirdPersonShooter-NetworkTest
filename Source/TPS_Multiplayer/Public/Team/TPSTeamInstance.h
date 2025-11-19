// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamDefinition.h"
#include "TPSTeamID.h"
#include "Army/TPSArmy.h"

#include "Army/Unit/TPSCommandUnit.h"
#include "World/TPSSpawnPool.h"

#include "TPSTeamInstance.generated.h"

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSTeamInstance();

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
    FTPSTeamConfiguration Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSArmyInstance>> Armies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSSpawnPool> SpawnPool;

//~ ==================================================================== ~//
//  LIVE STATE
//~ ==================================================================== ~//
public:
    // TODO: Create a "scoreboard" record for more detailed attribution.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    int ScoredPoints;

    // All characters that have *ever* been a part of this team
    //   (incl. deceased and never-spawned)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> Members;

    // All *active* (alive) characters that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> ActiveMembers;

    // All *active* (connected) players that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> ActivePlayers;
};


UCLASS()
class UTeamRosterPreset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTPSCommandUnit> CommandStructure;
};
