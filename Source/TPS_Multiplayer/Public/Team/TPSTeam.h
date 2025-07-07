// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"
#include "TPSCommandStructure.h"
#include "TPSTeamConfiguration.h"
#include "TPSTeamID.h"
#include "Game/TPSSpawnPoint.h"

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
    ETPSTeamID TeamID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FTPSTeamConfiguration Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSHierarchicalCollection> RootCollection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<ATPSSPawnPoint>> SpawnPoints;
};


UCLASS()
class UTeamRosterPreset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTPSCommandStructure> CommandStructure;


};
