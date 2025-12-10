// (C) ToasterCat Studios 2025

#pragma once

#include "TPSPlayerID.h"
#include "TPSPlayerState.h"

#include "TPSPlayerInstance.generated.h"

class UTPSCommandUnit;
class UTPSArmyInstance;
class UTPSTeamInstance;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdatePlayerInstance);


/**
 * Represents a Player in our ecosystem.
 *
 * Redundant wrapper around ATPSPlayerState used primarily for indexing and
 *   consistency with back-end player data persistence.
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSPlayerInstance : public UObject
{
    GENERATED_BODY()

public:
    UTPSPlayerInstance();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override { return true; }


//~ ======================================================================== ~//
//  COMPONENTS
//~ ======================================================================== ~//
public:

    // Broadcast Delegate
    UPROPERTY(BlueprintAssignable)
    FUpdatePlayerInstance PlayerInstanceUpdate;


//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    FTPSPlayerID PlayerID;

    // OwningPlayer
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_State)
    TWeakObjectPtr<ATPSPlayerState> State;
    UFUNCTION()
    void OnRep_State() { PlayerInstanceUpdate.Broadcast(); }




    ////////////////////////////////////////////////////////
    // Assignment Info (Player/Team/Army/Unit)
    //
    // TODO: Elevate to Assignable Interfaces [PC-237] [PC-236] [PC-257]
    //
    // UNUSED STUBS
    /*
protected:


    // Assigned Unit
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Assignment)
    TWeakObjectPtr<UTPSCommandUnit> AssignedUnit;
    // Assigned Army
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Assignment)
    TWeakObjectPtr<UTPSArmyInstance> AssignedArmy;
    // Assigned Team
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Assignment)
    TWeakObjectPtr<UTPSTeamInstance> AssignedTeam;

    UFUNCTION()
    void OnRep_Assignment();*/

};