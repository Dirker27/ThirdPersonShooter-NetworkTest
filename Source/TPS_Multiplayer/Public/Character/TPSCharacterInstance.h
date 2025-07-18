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
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
    TObjectPtr<ATPSCharacter> SpawnedActor;

public:
    UFUNCTION(BlueprintCallable)
    virtual void SpawnActor(TSubclassOf<ATPSCharacter> actorTemplate, AActor* spawnPoint);

    UFUNCTION(BlueprintCallable)
    virtual void DestroyActor();



private:
    UFUNCTION()
    void OnRep_Instigator() {};

private:
    UPROPERTY(ReplicatedUsing = OnRep_Instigator)
    TObjectPtr<UObject> Instigator;


};