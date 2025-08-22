// (C) ToasterCat Studios 2025

#pragma once

#include "TPSCharacter.h"
#include "TPSCharacterConfiguration.h"
#include "TPSCharacterIdentity.h"
#include "TPSCharacterRecord.h"
#include "Equipment/TPSEquipmentLoadout.h"

#include "TPSCharacterInstance.generated.h"

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
    virtual bool IsSupportedForNetworking() const override;


//~ ==================================================================== ~//
//  STATE
//~ ==================================================================== ~//

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterIdentity> Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterConfiguration> Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterRecord> Record;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSEquipmentLoadout> Loadout = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    bool IsAlive = true;



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