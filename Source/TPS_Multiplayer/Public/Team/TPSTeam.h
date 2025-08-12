// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSCommandStructure.h"
#include "TPSTeamConfiguration.h"
#include "TPSTeamID.h"

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
    TObjectPtr<UTPSCommandStructure> RootCollection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSSpawnPool> SpawnPool;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<int, TObjectPtr<UTPSCommandStructure>> CommandUnitsById;

    // All characters that have *ever* been a part of this team
    //   (incl. deceased and never-spawned)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<int, TObjectPtr<UTPSCharacterInstance>> MembersById;

    // All *active* (alive) characters that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<int, TObjectPtr<UTPSCharacterInstance>> ActiveMembersById;

    // All *active* (connected) players that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<int, TObjectPtr<UTPSCharacterInstance>> ActivePlayersById;
};


UCLASS()
class UTeamRosterPreset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTPSCommandStructure> CommandStructure;


};
