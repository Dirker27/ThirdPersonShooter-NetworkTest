// (C) ToasterCat Studios 2025

#pragma once

#include "CoreMinimal.h"

#include "Army/TPSArmyID.h"
#include "Army/TPSArmyIdentity.h"
#include "Army/Unit/TPSCommandUnit.h"
#include "Player/TPSPlayerID.h"
#include "Player/TPSPlayerState.h"
#include "Team/TPSTeamID.h"

#include "TPSArmy.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmyUpdate);

UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSArmyInstance : public UObject
{
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TEnumAsByte<ETPSTeamID> TeamID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    TObjectPtr<UTPSCommandUnit> RootUnit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    FTPSPlayerID OwningPlayer;


    // All characters that have *ever* been a part of this army
    //   (incl. deceased and never-spawned)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> Members;



    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCommandUnit>> Units;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
    TArray<TObjectPtr<UTPSCharacterInstance>> ActiveMembers;


//~ ==================================================================== ~//
//  OPERATIONS
//~ ==================================================================== ~//
public:
    UFUNCTION(BlueprintCallable)
    void Initialize(FTPSArmyID id, UTPSArmyIdentity* ident);

    UFUNCTION(BlueprintCallable)
    void AssignTeam(const ETPSTeamID team);

    UFUNCTION(BlueprintCallable)
    void AssignRootUnit(UTPSCommandUnit* unit);

    UFUNCTION(BlueprintPure)
    UTPSCommandUnit* GetRootUnit() { return RootUnit; }

    UFUNCTION(BlueprintCallable)
    void AddUnit(UTPSCommandUnit* unit);

    UFUNCTION(BlueprintCallable)
    void AddMember(UTPSCharacterInstance* instance);

    UFUNCTION(BlueprintCallable)
    void BindToPlayer(const FTPSPlayerID player);
};
