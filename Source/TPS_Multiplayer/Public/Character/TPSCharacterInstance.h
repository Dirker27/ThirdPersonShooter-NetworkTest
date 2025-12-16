// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacter.h"
#include "TPSCharacterConfiguration.h"
#include "TPSCharacterID.h"
#include "TPSCharacterIdentity.h"
#include "TPSCharacterRecord.h"

#include "Equipment/TPSEquipmentLoadout.h"
#include "Player/TPSPlayerState.h"
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
    friend class UTPSCommandUnit;

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
    FUpdateCharacterInstanceDisplay CharacterInstanceUpdate;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_IsAlive)
    bool IsAlive = true;

    UFUNCTION()
    void OnRep_IsAlive();


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    FTPSHitInfo LastHit;


    ////////////////////////////////////////////////////////
    // Assignment Info (Player/Team/Army/Unit)
    //
    // TODO: Elevate to Assignable Interfaces [PC-237] [PC-236] [PC-257]

protected:

    // OwningPlayer
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OwningPlayer)
    TWeakObjectPtr<ATPSPlayerState> OwningPlayer;
    UFUNCTION()
    void OnRep_OwningPlayer();


    // Assigned Unit
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_Assignment)
    TWeakObjectPtr<UTPSCommandUnit> AssignedUnit;
    // Assigned Army
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Assignment)
    TWeakObjectPtr<UTPSArmyInstance> AssignedArmy;
    // Assigned Team
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Assignment)
    TWeakObjectPtr<UTPSTeamInstance> AssignedTeam;

    UFUNCTION()
    void OnRep_Assignment();

public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSPlayerState* GetOwningPlayer() const { return OwningPlayer.Get(); }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCommandUnit* GetAssignedUnit() const { return AssignedUnit.Get(); }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSArmyInstance* GetAssignedArmy() const { return AssignedArmy.Get(); }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSTeamInstance* GetAssignedTeam() const { return AssignedTeam.Get(); }

    // Assigned Team ID
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ETPSTeamID GetAssignedTeamID() const;

    UFUNCTION(BlueprintCallable)
    void AssignToUnit(UTPSCommandUnit* unit);

    UFUNCTION(BlueprintCallable)
    void AssignToArmy(UTPSArmyInstance* army);

    UFUNCTION(BlueprintCallable)
    void AssignToTeam(UTPSTeamInstance* team);

    UFUNCTION(BlueprintCallable)
    void BindInstanceToPlayer(ATPSPlayerState* player);

    UFUNCTION(BlueprintCallable)
    void UnBindInstanceFromPlayer(ATPSPlayerState* player);

    UFUNCTION(BlueprintCallable)
    void ClearAssignment();




    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsActive() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsPossessedByPlayer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanBePossessed() const;


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
    TWeakObjectPtr<ATPSCharacter> SpawnedActor;

public:
    UFUNCTION(BlueprintCallable)
    virtual void SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint);

    UFUNCTION(BlueprintCallable)
    virtual void DestroyActor();

    UFUNCTION(BlueprintCallable)
    ATPSCharacter* GetSpawnedActor() const { return SpawnedActor.Get(); }



    //~ Inherited from Lyra ~//
private:
    UFUNCTION()
    void OnRep_Instigator() {};

private:
    UPROPERTY(ReplicatedUsing = OnRep_Instigator)
    TObjectPtr<UObject> Instigator;


};