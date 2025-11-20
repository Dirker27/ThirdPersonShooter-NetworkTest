// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacter.h"
#include "TPSCharacterConfiguration.h"
#include "TPSCharacterID.h"
#include "TPSCharacterIdentity.h"
#include "TPSCharacterRecord.h"

#include "Equipment/TPSEquipmentLoadout.h"
#include "Types/TPSHitInfo.h"

#include "TPSCharacterInstance.generated.h"

class UTPSCommandUnit;
class UTPSArmyInstance;
class UTPSTeamInstance;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCharacterInstanceDisplay);


/**
 * Describes a Character independent of their spawned actor representation.
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSCharacterInstance();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override { return true; }


//~ ======================================================================== ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:

    //////////////////////////////////////////////////////
    // Display Widgets

    // Broadcast Delegate
    UPROPERTY(BlueprintAssignable)
    FUpdateCharacterInstanceDisplay NotifyDisplayWidgets;


//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    FTPSCharacterID CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    FTPSOperatorIdentity Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    FTPSCharacterConfigurationData Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    FTPSOperatorRecord Record;



    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSEquipmentLoadout> Loadout = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    bool IsAlive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    FTPSHitInfo LastHit;


    ////////////////////////////////////////////////////////
    // Assignment Info (Team/Army/Unit)
    //
    // TODO: Migrate to "Assignment" Struct

    // Assigned Team ID
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ETPSTeamID GetAssignedTeamID() const;

    // Assigned Unit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TObjectPtr<UTPSCommandUnit> AssignedUnit;
    // Assigned Army
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TObjectPtr<UTPSArmyInstance> AssignedArmy;
    // Assigned Team
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TObjectPtr<UTPSTeamInstance> AssignedTeam;



//~ ======================================================================== ~//
//  Instance -> Actor Operations
//~ ======================================================================== ~//
public:
    UFUNCTION(BlueprintCallable)
    void Die();

    UFUNCTION(BlueprintCallable)
    void Focus();

    UFUNCTION(BlueprintCallable)
    void UnFocus();

    UFUNCTION(BlueprintCallable)
    void ShowDebug();

    UFUNCTION(BlueprintCallable)
    void HideDebug();

//~ ======================================================================== ~//
//  SPAWNED ACTOR(s) TRACKING
//~ ======================================================================== ~//
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
    TObjectPtr<ATPSCharacter> SpawnedActor;

public:
    UFUNCTION(BlueprintCallable)
    virtual void SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint);

    UFUNCTION(BlueprintCallable)
    virtual void DestroyActor();

    UFUNCTION(BlueprintCallable)
    ATPSCharacter* GetSpawnedActor() const { return SpawnedActor; }

private:
    UFUNCTION()
    void OnRep_Instigator() {};

private:
    UPROPERTY(ReplicatedUsing = OnRep_Instigator)
    TObjectPtr<UObject> Instigator;


};