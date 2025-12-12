// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "TPSTeamDefinition.h"
#include "TPSTeamID.h"

#include "Army/TPSArmy.h"
#include "World/Spawn/TPSSpawnPool.h"

#include "TPSTeamInstance.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeamUpdate);

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSTeamInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSTeamInstance();

    // Broadcast Delegate - Team/Character/Player Roster Updated
    UPROPERTY(BlueprintAssignable)
    FArmyUpdate TeamUpdate;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override { return true; }

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

protected:
    // Loose Team Members (Characters) that are not associated with an Army.
    //
    // TODO: Make Generic "TeamAssignable" for non-character units.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> Members;

    // All armies associated with this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TArray<TObjectPtr<UTPSArmyInstance>> Armies;

    // All *active* (connected) players that are a part of this team.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TArray<TObjectPtr<ATPSPlayerState>> Players;

public:
    UFUNCTION(BlueprintCallable)
    void AddMember(UTPSCharacterInstance* member);
    UFUNCTION(BlueprintCallable)
    void AddArmy(UTPSArmyInstance* army);
    UFUNCTION(BlueprintCallable)
    void AddPlayer(ATPSPlayerState* player);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<UTPSCharacterInstance*> GetMembers() const { return Members; }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<UTPSArmyInstance*> GetArmies() const { return Armies; }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<ATPSPlayerState*> GetActivePlayers() const { return Players; }





    ////////////////////////////////////////////////////////
    // Member Info
    //
    // TODO: [PC-258] Elevate to ManagedCollection Interface
public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetRandomActiveMember() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetRandomPossessableMember() const;
};
