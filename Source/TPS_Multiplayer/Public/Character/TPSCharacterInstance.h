// (C) ToasterCat Studios 2025

#pragma once

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterIdentity> Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterConfiguration> Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSCharacterRecord> Record;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTPSEquipmentLoadout> Loadout = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsAlive = false;


//~ ======================================================================== ~//
//  SPAWNED ACTOR(s) TRACKING
//~ ======================================================================== ~//
private:
    UPROPERTY(Replicated)
    TArray<TObjectPtr<AActor>> SpawnedActors;

public:
    UFUNCTION(BlueprintPure, Category = Equipment)
    TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

    UFUNCTION(BlueprintCallable)
    virtual void SpawnCharacterActors() { /* TODO */ }

    UFUNCTION(BlueprintCallable)
    virtual void DestroyCharacterActors() { /* TODO */ }



private:
    UFUNCTION()
    void OnRep_Instigator() {};

private:
    UPROPERTY(ReplicatedUsing = OnRep_Instigator)
    TObjectPtr<UObject> Instigator;


};