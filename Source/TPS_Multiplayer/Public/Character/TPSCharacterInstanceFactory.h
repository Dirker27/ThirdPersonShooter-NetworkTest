// (C) ToasterCat Studios 2025

#pragma once

#include "Game/TPSGameState.h"

#include "TPSCharacterInstanceFactory.generated.h"

/**
 * GameMode worker sub-component to create new Character Instances, Identities, and Actors.
 *
 * TODO: Derive names and similar info from Faction-appropriate name pools.
 *   - https://toastercatstudios.atlassian.net/browse/PC-225
 */
UCLASS(BlueprintType)
class TPS_MULTIPLAYER_API UTPSCharacterInstanceFactory : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSCharacterInstanceFactory();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UTPSCharacterConfiguration> DefaultCharacterTemplate;

public:
    static UTPSCharacterInstance* ConfigureCharacterInstanceForUnitAndRole(UTPSCharacterInstance* instance, FTPSUnitID unit, FTPSUnitRoleDefinitionData roleDefinition);
};

