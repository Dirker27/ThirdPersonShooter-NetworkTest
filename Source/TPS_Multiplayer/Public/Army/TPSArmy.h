// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Army/TPSArmyID.h"
#include "Army/TPSArmyIdentity.h"
#include "Army/Unit/TPSCommandUnit.h"
#include "Player/TPSPlayerState.h"
#include "Team/TPSTeamID.h"

#include "TPSArmy.generated.h"


class UTPSTeamInstance;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmyUpdate);

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSArmyInstance : public UObject
{
    friend class UTPSCommandUnit;

    GENERATED_BODY()

public:
    UTPSArmyInstance() {};

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    FTPSArmyID ArmyID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TObjectPtr<UTPSArmyIdentity> Identity;


//~ ==================================================================== ~//
//  LIVE STATE
//~ ==================================================================== ~//
private:
    bool IsInitialized = false;

protected:
    // Top-Level unit of the army - the Root of the Hierarchy Trie.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TObjectPtr<UTPSCommandUnit> RootUnit;

    // The player who currently owns this army.
    //   Armies will be exclusively owned by ONE[1] Player.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TWeakObjectPtr<ATPSPlayerState> OwningPlayer;



    // All characters that have *ever* been a part of this army
    //   (incl. deceased and never-spawned)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> Members;

    // Characters that are currently alive and interacting in the game world.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> ActiveMembers;

    // All command-able units under this army's jurisdiction.
    //   (redundant index for access optimization)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCommandUnit>> Units;




    ////////////////////////////////////////////////////////
    // Assignment Info (Team/Army/Unit)
    //
    // TODO: Elevate to Assignable Interfaces [PC-237] [PC-236] [PC-257]
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    TWeakObjectPtr<UTPSTeamInstance> AssignedTeam;

public:
    // Assigned Team Instance
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSTeamInstance* GetAssignedTeam() const;

    // Assigned Team ID
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ETPSTeamID GetAssignedTeamID() const;

    // Assigned Player
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ATPSPlayerState* GetOwningPlayer() const { return OwningPlayer.Get(); }


    ////////////////////////////////////////////////////////
    // Member Info
    //
    // TODO: [PC-258] Elevate to ManagedCollection Interface
public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetRandomActiveMember() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UTPSCharacterInstance* GetRandomPossessableMember() const;

//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:
    UFUNCTION(BlueprintCallable)
    void Initialize(FTPSArmyID id, UTPSArmyIdentity* ident);

    UFUNCTION(BlueprintCallable)
    void AssignToTeam(UTPSTeamInstance* team);

    UFUNCTION(BlueprintCallable)
    void AssignRootUnit(UTPSCommandUnit* unit);

    UFUNCTION(BlueprintPure)
    UTPSCommandUnit* GetRootUnit() { return RootUnit; }

    UFUNCTION(BlueprintCallable)
    void AddUnit(UTPSCommandUnit* unit);

    UFUNCTION(BlueprintCallable)
    void AddMember(UTPSCharacterInstance* instance);

    UFUNCTION(BlueprintCallable)
    TArray<UTPSCharacterInstance*> GetMembers() const { return Members;};

    UFUNCTION(BlueprintCallable)
    void BindToPlayer(ATPSPlayerState* player);
};
